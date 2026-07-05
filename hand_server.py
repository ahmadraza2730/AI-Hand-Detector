import cv2
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision
import socket
import json
import urllib.request
import os
import time

print("Starting Hand Server...")

model_path = "hand_landmarker.task"
if not os.path.exists(model_path):
    print("Downloading model (~30MB)...")
    url = "https://storage.googleapis.com/mediapipe-models/hand_landmarker/hand_landmarker/float16/latest/hand_landmarker.task"
    urllib.request.urlretrieve(url, model_path)
    print("Downloaded!")

base_options = python.BaseOptions(
    model_asset_path=model_path)
options = vision.HandLandmarkerOptions(
    base_options=base_options,
    running_mode=vision.RunningMode.VIDEO,
    num_hands=2,
    min_hand_detection_confidence=0.5,
    min_hand_presence_confidence=0.5,
    min_tracking_confidence=0.5)
landmarker = vision.HandLandmarker.create_from_options(options)

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.bind(('127.0.0.1', 5000))
server.listen(1)
print("Waiting for C++ client...")
conn, addr = server.accept()
print("C++ connected!")

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
cap.set(cv2.CAP_PROP_BRIGHTNESS, 150)
print("Camera opened! Show your hand...")

CONN = [
    (0,1),(1,2),(2,3),(3,4),
    (0,5),(5,6),(6,7),(7,8),
    (0,9),(9,10),(10,11),(11,12),
    (0,13),(13,14),(14,15),(15,16),
    (0,17),(17,18),(18,19),(19,20)
]

while True:
    ret, frame = cap.read()
    if not ret:
        break

    h, w = frame.shape[:2]
    frame = cv2.flip(frame, 1)
    frame = cv2.convertScaleAbs(frame, alpha=1.2, beta=20)

    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    mp_image = mp.Image(
        image_format=mp.ImageFormat.SRGB, data=rgb)

    # Use VIDEO mode for smooth tracking
    timestamp_ms = int(time.time() * 1000)
    result = landmarker.detect_for_video(
        mp_image, timestamp_ms)

    data = {"hands": []}

    if result.hand_landmarks:
        for hand_lms in result.hand_landmarks:
            lm_list = [
                {"x": round(lm.x, 6),
                 "y": round(lm.y, 6),
                 "z": round(lm.z, 6)}
                for lm in hand_lms
            ]
            data["hands"].append(lm_list)

            # Draw skeleton lines
            for c in CONN:
                x1 = int(hand_lms[c[0]].x * w)
                y1 = int(hand_lms[c[0]].y * h)
                x2 = int(hand_lms[c[1]].x * w)
                y2 = int(hand_lms[c[1]].y * h)
                cv2.line(frame,
                    (x1,y1),(x2,y2),
                    (0,255,255), 2)

            # Draw dots
            for i, lm in enumerate(hand_lms):
                cx = int(lm.x * w)
                cy = int(lm.y * h)
                if i == 0:
                    color, r = (255,0,0), 8
                elif i in [4,8,12,16,20]:
                    color, r = (0,0,255), 8
                else:
                    color, r = (0,255,0), 5
                cv2.circle(frame, (cx,cy), r, color, -1)

    # Status
    status = f"Hands: {len(data['hands'])}"
    color = (0,255,0) if data['hands'] else (0,100,255)
    cv2.putText(frame, status,
        (10,40), cv2.FONT_HERSHEY_SIMPLEX,
        1.2, color, 2)

    cv2.imshow("Python MediaPipe Server", frame)

    try:
        conn.sendall(
            (json.dumps(data)+"\n").encode('utf-8'))
    except:
        print("C++ disconnected")
        break

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
conn.close()
server.close()
print("Done.")