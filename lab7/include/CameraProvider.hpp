#pragma once
#include <opencv2/opencv.hpp>

class CameraProvider {
private:
    cv::VideoCapture cap; // Поле для роботи з камерою

public:
    CameraProvider();
    ~CameraProvider();
    cv::Mat getFrame(); // Метод повертає кадр
};