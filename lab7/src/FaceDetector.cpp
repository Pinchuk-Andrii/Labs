#include "FaceDetector.hpp"

FaceDetector::FaceDetector() {
    // Завантаження мережі 
    net = cv::dnn::readNetFromCaffe("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");
    // Запуск фонового потоку 
    worker = std::thread(&FaceDetector::detectLoop, this);
}

FaceDetector::~FaceDetector() {
    running = false; // Безпечна зупинка
    if (worker.joinable()) worker.join();
}

void FaceDetector::updateFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(frameMutex); // Блокування для запису
    currentFrame = frame.clone();
    hasNewFrame = true;
}

std::vector<cv::Rect> FaceDetector::getFaces() {
    std::lock_guard<std::mutex> lock(frameMutex); // Блокування для читання
    return detectedFaces;
}

void FaceDetector::detectLoop() {
    while (running) {
        cv::Mat frame;
        {
            std::lock_guard<std::mutex> lock(frameMutex);
            if (hasNewFrame) {
                frame = currentFrame.clone();
                hasNewFrame = false;
            }
        }

        if (!frame.empty()) {
            // Підготовка blob (300x300)
            cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0));
            net.setInput(blob);
            cv::Mat detections = net.forward(); // Детекція
            std::vector<cv::Rect> faces;
            cv::Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());
            
            for (int i = 0; i < detectionMat.rows; i++) {
                float confidence = detectionMat.at<float>(i, 2);
                if (confidence > 0.5) {
                    int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                    int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                    int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                    int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);
                    faces.push_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
                }
            }
            
            // Навантаження для демонстрації плавності (Рівень 2)
            // std::this_thread::sleep_for(std::chrono::milliseconds(200));

            {
                std::lock_guard<std::mutex> lock(frameMutex);
                detectedFaces = faces; // Оновлення результатів
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Пауза циклу
    }
}