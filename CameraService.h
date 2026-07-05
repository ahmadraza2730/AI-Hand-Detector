#pragma once
#include <opencv2/opencv.hpp>

class CameraService {
private:
    cv::VideoCapture _cap;
    int _deviceId;

public:
    CameraService(int deviceId = 0);
    bool start();
    bool getFrame(cv::Mat& frame);
    void stop();
    bool isOpen() const;
};