cmake_minimum_required(VERSION 3.10)
project(integral)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опция для выбора типа массива

find_package(OpenMP REQUIRED)
if(NOT OpenMP_CXX_FOUND)
	message(SEND_ERROR "Failed to find OpenMP.")
	return()
else()
	include_directories(${OpenMP_INCLUDE_DIRS})
    message("OpenMP FOUND dir is ${OpenMP_INCLUDE_DIRS}.")
endif()


add_executable(integral main.cpp)

target_link_libraries(integral OpenMP::OpenMP_CXX)

# Установка флагов оптимизации
target_compile_options(integral PRIVATE -O3)
