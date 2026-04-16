#include "CameraProvider.hpp"
#include "FrameProcessor.hpp"
#include "KeyProcessor.hpp"
#include "Display.hpp"

int main() {
    // Ініціалізація модулів 
    CameraProvider camera;
    FrameProcessor processor;
    KeyProcessor keys;
    Display display("OpenCV Video Processing");

    while (true) {
        // Отримуємо кадр з камери
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        // Чекаємо на натискання клавіші (30 мс)
        int key = cv::waitKey(30);
        
        // Визначаємо режим обробки за клавішею 
        ProcessMode mode = keys.getMode(key);
        
        // Обробляємо кадр відповідно до режиму 
        cv::Mat processedFrame = processor.process(frame, mode);
        
        // Виводимо результат у вікно 
        display.show(processedFrame);

        // Вихід з програми на 'q' або Esc
        if (key == 'q' || key == 27) break;
    }
    return 0;
}