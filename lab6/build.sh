#!/bin/bash
echo "Починаємо компіляцію проєкту"
mkdir -p build
cd build
cmake ..
make
echo "Компіляція успішна"