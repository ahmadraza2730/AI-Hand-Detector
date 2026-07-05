#include "GestureRecognizer.h"

int GestureRecognizer::countFingers(
    const std::vector<HandLandmark>& lm) {
    if ((int)lm.size() < 21) return 0;
    int count = 0;
    if (lm[_tipIds[0]].x > lm[_tipIds[0] - 1].x)
        count++;
    for (int i = 1; i < 5; i++) {
        if (lm[_tipIds[i]].y < lm[_tipIds[i] - 2].y)
            count++;
    }
    return count;
}

std::string GestureRecognizer::getGestureName(int count) {
    switch (count) {
    case 0:  return "Fist";
    case 1:  return "One";
    case 2:  return "Peace";
    case 3:  return "Three";
    case 4:  return "Four";
    case 5:  return "Open Hand";
    default: return "Unknown";
    }
}