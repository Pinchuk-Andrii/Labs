#!/bin/bash
# Оновлення та встановлення OpenCV, CMake та компілятора
sudo apt update
sudo apt install libopencv-dev cmake gcc g++ wget -y

# Завантаження архітектури нейромережі 
wget https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt

# Завантаження ваг моделі
wget https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel