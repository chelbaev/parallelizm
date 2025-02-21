создание папки проекта:

mkdir build
cd build


float компиляция из папки build:

cmake ..
make


double компиляция из папки build:

cmake -DUSE_DOUBLE=ON ..
make


запуск из папки build: ./sin_sum
