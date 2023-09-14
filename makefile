CXX = clang++
CXXFLAGS = -std=c++11 -Wall

all: main

main: main.cpp
		$(CXX) $(CXXFLAGS) -o $@ $<

clean:
		rm -f ./main