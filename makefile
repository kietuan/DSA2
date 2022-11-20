CXX = g++
FLAG = -pedantic-errors -Werror -g -O0


all: 
	$(CXX) main.cpp ConcatStringTree.cpp $(FLAG) -I . -std=c++11 -o main
	./main

run: main
	./main

test: test.cpp
	$(CXX) $(FLAG)  -std=c++11 test.cpp -o test 
	./test

clean:
	rm main
	rm test