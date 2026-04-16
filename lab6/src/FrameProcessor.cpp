#include "FrameProcessor.hpp"

cv::Mat FrameProcessor::process(const cv::Mat& inputFrame, ProcessMode mode) {
    if (inputFrame.empty()) return inputFrame;
    cv::Mat outputFrame = inputFrame.clone();

    switch (mode) {
        case ProcessMode::INVERSION:
            cv::bitwise_not(inputFrame, outputFrame);
            break;

        case ProcessMode::BLUR:
            cv::GaussianBlur(inputFrame, outputFrame, cv::Size(15, 15), 0);
            break;

        case ProcessMode::CANNY:
            cv::Canny(inputFrame, outputFrame, 100, 200);
            cv::cvtColor(outputFrame, outputFrame, cv::ColorConversionCodes::COLOR_GRAY2BGR);
            break;

        case ProcessMode::NORMAL:
        default:
            // Повертаємо оригінал без змін
            break;
    }
    return outputFrame;
}