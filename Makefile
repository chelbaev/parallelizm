CXX = g++
CXXFLAGS = -Wall -Wextra -O3 -std=c++17

# Определение типа массива (по умолчанию float)
ARRAY_TYPE ?= float

ifeq ($(ARRAY_TYPE), double)
    CXXFLAGS += -DUSE_DOUBLE
endif

TARGET = sin_sum

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

clean:
	rm -f $(TARGET)