CXX = g++
FLAG = -pedantic-errors -Werror

all: 
	$(CXX) main.cpp ConcatStringTree.cpp $(FLAG) -I . -std=c++11  -o main
