#!/bin/bash
echo "Встановлюємо залежності"
sudo apt update
sudo apt install libopencv-dev cmake gcc g++ make -y
echo "Готово"