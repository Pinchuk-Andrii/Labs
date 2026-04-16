#!/bin/bash
# Створюємо папку build, заходимо в неї, білдимо проєкт
mkdir -p build
cd build
cmake ..
make