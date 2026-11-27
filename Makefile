CXX = g++
CXXFLAGS = -g -Wall -Wextra -std=c++11

matrix: matrix.cpp
	$(CXX) $(CXXFLAGS) -o matrix matrix.cpp

clean:
	rm -f matrix results.txt

.PHONY: clean