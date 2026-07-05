#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

struct HandLandmark {
    float x, y, z;
};

class HandDetector {
private:
    int _maxHands;
    SOCKET _sock;
    bool _connected;
    std::string _buffer;
    std::vector<std::vector<HandLandmark>> _landmarks;

    bool connectToServer();
    bool parseLandmarks(const std::string& json);

public:
    HandDetector(float detConf = 0.7f, int maxHands = 2);
    ~HandDetector();

    bool loadModels(const std::string& palmModel,
        const std::string& landmarkModel);

    cv::Mat detect(cv::Mat& frame);
    void drawLandmarks(cv::Mat& frame);
    std::vector<std::vector<HandLandmark>> getLandmarks() const;
    int getHandCount() const;
};