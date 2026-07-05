# 🖐️ AI Hand Detector

A real-time hand detection and tracking application built using **C++** and **Python**, combining computer vision techniques to identify and track hand landmarks and gestures.

## 📖 About

This project explores real-time computer vision by implementing a hand detection pipeline that identifies hand position, landmarks, and gestures from a live camera feed. Built as a hybrid C++/Python project, it demonstrates cross-language integration — leveraging Python's rich CV/ML ecosystem alongside C++'s performance for real-time processing.

## ✨ Features

- 🎥 Real-time hand detection from webcam feed
- 📍 Hand landmark tracking
- 🤖 Gesture recognition capability
- ⚡ Optimized processing pipeline combining C++ and Python
- 🖼️ Live visual overlay of detected hand points

## 🛠️ Tech Stack

- **Languages:** C++, Python
- **Libraries:** OpenCV, MediaPipe *(update if different)*
- **Concepts used:** Computer Vision, Real-time Video Processing, Landmark Detection

## 🚀 Getting Started

### Prerequisites
- Python 3.x
- OpenCV (`pip install opencv-python`)
- MediaPipe (`pip install mediapipe`)
- A C++ compiler (for the C++ components)

### Run Locally

**Python version:**
```bash
git clone https://github.com/ahmadraza2730/AI-Hand-Detector.git
cd AI-Hand-Detector
pip install -r requirements.txt
python hand_detector.py
```

**C++ version:**
```bash
g++ hand_detector.cpp -o HandDetector `pkg-config --cflags --libs opencv4`
./HandDetector
```

## 🎓 Why I Built This

I built this project to explore practical applications of computer vision and understand how detection models process real-time video data — bridging my C++ foundations with Python's ML/CV ecosystem.

## 📌 Author

**Muhammad Ahmad**
CS Student @ FAST-NUCES | C++ & Python Developer | Exploring CV & ML
