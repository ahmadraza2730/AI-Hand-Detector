#include "HandDetector.h"
#include "GestureRecognizer.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    HandDetector detector(0.7f, 2);
    GestureRecognizer recognizer;

    std::cout << "Starting Hand Detection App...\n";
    std::cout << "Make sure hand_server.py is running!\n";

    if (!detector.loadModels("", "")) {
        std::cerr << "Failed to connect to Python!\n";
        std::cerr << "Run: python hand_server.py\n";
        return -1;
    }

    std::cout << "Running! Press Q to quit.\n";

    // C++ creates a blank frame to draw on
    // Python handles the camera
    cv::Mat frame(480, 640, CV_8UC3,
        cv::Scalar(30, 30, 30));

    while (true) {
        // Reset frame each loop
        frame = cv::Scalar(30, 30, 30);

        // Get landmarks from Python and draw them
        frame = detector.detect(frame);

        auto landmarks = detector.getLandmarks();

        if (!landmarks.empty()) {
            int fingers =
                recognizer.countFingers(landmarks[0]);
            std::string gesture =
                recognizer.getGestureName(fingers);

            cv::putText(frame,
                "Fingers: " + std::to_string(fingers),
                cv::Point(30, 60),
                cv::FONT_HERSHEY_SIMPLEX,
                1.5, cv::Scalar(0, 255, 0), 3);

            cv::putText(frame, gesture,
                cv::Point(30, 110),
                cv::FONT_HERSHEY_SIMPLEX,
                1.2, cv::Scalar(255, 200, 0), 2);

            cv::putText(frame,
                "Hands: " +
                std::to_string(detector.getHandCount()),
                cv::Point(30, 155),
                cv::FONT_HERSHEY_SIMPLEX,
                0.8, cv::Scalar(200, 200, 255), 2);
        }
        else {
            cv::putText(frame, "Show your hand...",
                cv::Point(30, 60),
                cv::FONT_HERSHEY_SIMPLEX,
                1.0, cv::Scalar(0, 100, 255), 2);
        }

        cv::putText(frame, "C++ OOP Hand Detection",
            cv::Point(30, frame.rows - 20),
            cv::FONT_HERSHEY_SIMPLEX,
            0.6, cv::Scalar(150, 150, 150), 1);

        cv::imshow("Hand Detection App (C++ OOP)", frame);

        if (cv::waitKey(1) == 'q') break;
    }

    cv::destroyAllWindows();
    return 0;
}