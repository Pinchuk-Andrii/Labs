#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex> 
#include <atomic>
#include <vector>

class FaceDetector {
private:
    cv::dnn::Net net;
    std::thread worker; // Потік детекції
    std::mutex frameMutex; // М'ютекс для захисту кадру та результатів
    std::atomic<bool> running{true}; // Прапорець зупинки
    cv::Mat currentFrame;
    std::vector<cv::Rect> detectedFaces;
    bool hasNewFrame = false;
    void detectLoop(); // Внутрішній цикл потоку

public:
    FaceDetector();
    ~FaceDetector();
    void updateFrame(const cv::Mat& frame); // Отримання кадру від UI потоку
    std::vector<cv::Rect> getFaces();       // Передача координат у UI потік
};