CXX = g++
FLAG = -pedantic-errors -Werror

all: 
	$(CXX) -o main main.cpp ConcatStringTree.cpp $(FLAG) -I . -std=c++11

run: main
	./main

clean:
	rm main