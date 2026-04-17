#pragma once

enum class ProcessMode {
    NORMAL,
    INVERSION,
    BLUR,
    CANNY,
    FACE
};

class KeyProcessor {
private:
    ProcessMode currentMode = ProcessMode::NORMAL;

public:
    ProcessMode getMode(int key) {
        switch (key) {
            case '1': currentMode = ProcessMode::NORMAL; break;
            case '2': currentMode = ProcessMode::INVERSION; break;
            case '3': currentMode = ProcessMode::BLUR; break;
            case '4': currentMode = ProcessMode::CANNY; break;
            case 'f': currentMode = ProcessMode::FACE; break; //
            default: break;
        }
        return currentMode;
    }
};