cmake_minimum_required(VERSION 3.10)
project(Lab6_OpenCV)

# Стандарт C++17
set(CMAKE_CXX_STANDARD 17)

# Пошук OpenCV
find_package(OpenCV REQUIRED)

# Додаємо папку include для заголовних файлів
include_directories(include ${OpenCV_INCLUDE_DIRS})

# Збираємо всі .cpp файли з src
file(GLOB SOURCES "src/*.cpp")

# Створюємо програму
add_executable(lab6_app ${SOURCES})

# Лінкуємо бібліотеки OpenCV
target_link_libraries(lab6_app ${OpenCV_LIBS})