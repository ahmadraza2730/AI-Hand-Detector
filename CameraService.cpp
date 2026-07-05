#include "CameraService.h"
#include <iostream>

CameraService::CameraService(int deviceId)
    : _deviceId(deviceId) {
}

bool CameraService::start() {
    _cap.open(_deviceId);
    if (!_cap.isOpened()) {
        std::cerr << "Error: Cannot open camera\n";
        return false;
    }
    _cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    _cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    return true;
}

bool CameraService::getFrame(cv::Mat& frame) {
    return _cap.read(frame);
}

void CameraService::stop() {
    _cap.release();
}

bool CameraService::isOpen() const {
    return _cap.isOpened();
}