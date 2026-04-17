#include "FrameProcessor.hpp"
#include <opencv2/dnn.hpp> // Потрібно для роботи з нейронками 

cv::dnn::Net net = cv::dnn::readNetFromCaffe("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");
cv::Mat FrameProcessor::process(const cv::Mat& inputFrame, ProcessMode mode) {
    if (inputFrame.empty()) return inputFrame;
    cv::Mat outputFrame = inputFrame.clone();
    if (mode == ProcessMode::FACE) {
        // Перетворення кадру в blob (розмір 300x300)
        cv::Mat blob = cv::dnn::blobFromImage(inputFrame, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0));
        net.setInput(blob);
        
        // Запуск детекції
        cv::Mat detections = net.forward();
        
        // Обробка результатів
        cv::Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());

        for (int i = 0; i < detectionMat.rows; i++) {
            float confidence = detectionMat.at<float>(i, 2);

            // Фільтруємо об'єкти з впевненістю > 50%
            if (confidence > 0.5) {
                int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * inputFrame.cols);
                int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * inputFrame.rows);
                int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * inputFrame.cols);
                int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * inputFrame.rows);

                // Малюємо рамку навколо обличчя 
                cv::rectangle(outputFrame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2);
            }
        }
        return outputFrame;
    }

    switch (mode) {
        case ProcessMode::INVERSION: cv::bitwise_not(inputFrame, outputFrame); break;
        case ProcessMode::BLUR: cv::GaussianBlur(inputFrame, outputFrame, cv::Size(15, 15), 0); break;
        case ProcessMode::CANNY: 
            cv::Canny(inputFrame, outputFrame, 100, 200);
            cv::cvtColor(outputFrame, outputFrame, cv::COLOR_GRAY2BGR);
            break;
        default: break;
    }
    return outputFrame;
}