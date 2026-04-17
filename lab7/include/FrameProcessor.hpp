#pragma once
#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
public:
    // Основний метод обробки кадру 
    cv::Mat process(const cv::Mat& inputFrame, ProcessMode mode);
};