cmake_minimum_required(VERSION 3.10)
project(SinSum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опция для выбора типа массива
option(USE_DOUBLE "Use double precision" OFF)

if(USE_DOUBLE)
    add_definitions(-DUSE_DOUBLE)
endif()

add_executable(sin_sum main.cpp)

# Установка флагов оптимизации
target_compile_options(sin_sum PRIVATE -O3)