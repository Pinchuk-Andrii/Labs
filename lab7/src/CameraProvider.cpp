#include "CameraProvider.hpp"
#include <iostream>

CameraProvider::CameraProvider() {
    // Відкриваємо стандартну камеру (індекс 0)
    if (!cap.open(0)) {
        std::cerr << "Помилка: не вдалося відкрити камеру!" << std::endl;
    }
}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) {
        cap.release(); // Звільняємо ресурс при видаленні об'єкта
    }
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap >> frame; // Записуємо поточний кадр з камери в об'єкт Mat
    return frame;
}