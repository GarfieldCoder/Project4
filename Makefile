CXX = g++
CXXFLAGS = -g -Wall -std=c++11

TARGET = matrix

all: $(TARGET)

$(TARGET): matrix.cpp
	$(CXX) $(CXXFLAGS) matrix.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean