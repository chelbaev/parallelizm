cmake_minimum_required(VERSION 3.10)
project(TaskServer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем исполняемый файл
add_executable(task_server
    main.cpp
)

# Устанавливаем свойства для предупреждений и оптимизации
if(MSVC)
    target_compile_options(task_server PRIVATE /W4 /O2)
else()
    target_compile_options(task_server PRIVATE -Wall -Wextra -pedantic -O3)
endif()

# Для Windows может потребоваться явно указать pthread
if(UNIX AND NOT APPLE)
    target_link_libraries(task_server PRIVATE pthread)
endif()
