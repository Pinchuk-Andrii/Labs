#include "CameraProvider.hpp"
#include "FrameProcessor.hpp"
#include "KeyProcessor.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"

int main() {
    CameraProvider camera;
    FrameProcessor processor;
    KeyProcessor keys;
    Display display("Lab 7 async face detection");
    FaceDetector detector;
    while (true) {
        cv::Mat frame = camera.getFrame(); // Читаємо кадр
        if (frame.empty()) break;
        int key = cv::waitKey(30);
        ProcessMode mode = keys.getMode(key);      
        cv::Mat outputFrame;

        if (mode == ProcessMode::FACE) {
            detector.updateFrame(frame); // Віддаємо копію кадру
            auto faces = detector.getFaces(); // Забираємо координати без очікування          
            outputFrame = frame.clone();
            for (const auto& rect : faces) {
                cv::rectangle(outputFrame, rect, cv::Scalar(0, 255, 0), 2); // Малюємо рамки
            }
        } else {
            outputFrame = processor.process(frame, mode);
        }     
        display.show(outputFrame); // Відображаємо
        if (key == 'q' || key == 27) break;
    }
    return 0;
}