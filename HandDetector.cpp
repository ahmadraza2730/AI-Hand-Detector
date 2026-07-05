#include "HandDetector.h"
#include <iostream>

HandDetector::HandDetector(float detConf, int maxHands)
    : _maxHands(maxHands), _connected(false), _sock(INVALID_SOCKET) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
}

HandDetector::~HandDetector() {
    if (_connected) {
        closesocket(_sock);
    }
    WSACleanup();
}

bool HandDetector::loadModels(
    const std::string& palmModel,
    const std::string& landmarkModel) {
    return connectToServer();
}

bool HandDetector::connectToServer() {
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        return false;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    if (connect(_sock,
        (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Cannot connect to Python server!\n";
        closesocket(_sock);
        return false;
    }

    u_long mode = 1;
    ioctlsocket(_sock, FIONBIO, &mode);

    _connected = true;
    std::cout << "Connected to Python hand server!\n";
    return true;
}

bool HandDetector::parseLandmarks(const std::string& json) {
    _landmarks.clear();

    if (json.find("\"hands\":[]") != std::string::npos)
        return true;

    size_t pos = 0;
    std::vector<HandLandmark> currentHand;

    while ((pos = json.find("\"x\":", pos)) != std::string::npos) {
        try {
            HandLandmark lm;

            pos += 4;
            size_t xEnd = json.find_first_of(",}", pos);
            lm.x = std::stof(json.substr(pos, xEnd - pos));

            size_t yPos = json.find("\"y\":", pos);
            if (yPos == std::string::npos) break;
            yPos += 4;
            size_t yEnd = json.find_first_of(",}", yPos);
            lm.y = std::stof(json.substr(yPos, yEnd - yPos));

            size_t zPos = json.find("\"z\":", yPos);
            if (zPos == std::string::npos) break;
            zPos += 4;
            size_t zEnd = json.find_first_of(",}", zPos);
            lm.z = std::stof(json.substr(zPos, zEnd - zPos));

            currentHand.push_back(lm);
            pos = zPos + 4;

            if ((int)currentHand.size() == 21) {
                _landmarks.push_back(currentHand);
                currentHand.clear();
                if ((int)_landmarks.size() >= _maxHands)
                    break;
            }
        }
        catch (...) {
            break;
        }
    }

    return true;
}

cv::Mat HandDetector::detect(cv::Mat& frame) {
    if (!_connected) return frame;

    char buf[65536];
    int received = recv(_sock, buf, sizeof(buf) - 1, 0);

    if (received > 0) {
        buf[received] = '\0';
        _buffer += buf;

        size_t newline;
        while ((newline = _buffer.find('\n')) != std::string::npos) {
            std::string line = _buffer.substr(0, newline);
            _buffer = _buffer.substr(newline + 1);
            if (!line.empty())
                parseLandmarks(line);
        }
    }
    else if (received == 0) {
        std::cerr << "Python server disconnected\n";
        _connected = false;
    }

    drawLandmarks(frame);
    return frame;
}

void HandDetector::drawLandmarks(cv::Mat& frame) {
    std::vector<std::pair<int, int>> connections = {
        {0,1},{1,2},{2,3},{3,4},
        {0,5},{5,6},{6,7},{7,8},
        {0,9},{9,10},{10,11},{11,12},
        {0,13},{13,14},{14,15},{15,16},
        {0,17},{17,18},{18,19},{19,20}
    };

    for (auto& hand : _landmarks) {
        if ((int)hand.size() < 21) continue;

        for (auto& c : connections) {
            int x1 = (int)(hand[c.first].x * frame.cols);
            int y1 = (int)(hand[c.first].y * frame.rows);
            int x2 = (int)(hand[c.second].x * frame.cols);
            int y2 = (int)(hand[c.second].y * frame.rows);
            cv::line(frame,
                cv::Point(x1, y1),
                cv::Point(x2, y2),
                cv::Scalar(0, 255, 255), 2);
        }

        for (int i = 0; i < 21; i++) {
            int cx = (int)(hand[i].x * frame.cols);
            int cy = (int)(hand[i].y * frame.rows);

            bool isTip = (i == 4 || i == 8 || i == 12 || i == 16 || i == 20);
            bool isWrist = (i == 0);

            cv::Scalar color;
            int radius;

            if (isWrist) {
                color = cv::Scalar(255, 0, 0);
                radius = 8;
            }
            else if (isTip) {
                color = cv::Scalar(0, 0, 255);
                radius = 8;
            }
            else {
                color = cv::Scalar(0, 255, 0);
                radius = 5;
            }

            cv::circle(frame, cv::Point(cx, cy),
                radius, color, cv::FILLED);
        }
    }
}

std::vector<std::vector<HandLandmark>>
HandDetector::getLandmarks() const {
    return _landmarks;
}

int HandDetector::getHandCount() const {
    return static_cast<int>(_landmarks.size());
}