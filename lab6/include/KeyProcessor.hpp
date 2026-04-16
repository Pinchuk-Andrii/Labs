#pragma once

// Перелік режимів обробки зображення 
enum class ProcessMode {
    NORMAL,      // Звичайне відео
    INVERSION,   // Інверсія кольорів
    BLUR,        // Розмиття за Гаусом
    CANNY        // Детектор меж Канні
};

class KeyProcessor {
private:
    ProcessMode currentMode = ProcessMode::NORMAL;

public:
    // Повертає обраний режим на основі коду натиснутої клавіші
    ProcessMode getMode(int key) {
        switch (key) {
            case '1': currentMode = ProcessMode::NORMAL; break;
            case '2': currentMode = ProcessMode::INVERSION; break;
            case '3': currentMode = ProcessMode::BLUR; break;
            case '4': currentMode = ProcessMode::CANNY; break;
            default: break; // Якщо інша клавіша режим не змінюється
        }
        return currentMode;
    }
};