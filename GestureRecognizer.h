#pragma once
#include "HandDetector.h"
#include <string>

class BaseRecognizer {
public:
    virtual int countFingers(
        const std::vector<HandLandmark>& lm) = 0;
    virtual std::string getGestureName(int count) = 0;
    virtual ~BaseRecognizer() = default;
};

class GestureRecognizer : public BaseRecognizer {
private:
    int _tipIds[5] = { 4, 8, 12, 16, 20 };
public:
    int countFingers(
        const std::vector<HandLandmark>& lm) override;
    std::string getGestureName(int count) override;
};