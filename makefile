CXX = g++
FLAG = -pedantic-errors -Werror -g -O0


all: 
	$(CXX) main.cpp ConcatStringTree.cpp  $(FLAG) -I . -std=c++11 -o main.o
	./main.o
	rm main.o

concat:
	$(CXX) testConcat.cpp ConcatStringTree.cpp  $(FLAG) -I . -std=c++11 -o testConcat.o
	./testConcat.o
	rm testConcat.o

reduced:
	$(CXX) testReduced.cpp ReducedConcatStringTree.cpp  $(FLAG) -I . -std=c++11 -o testReduced.o
	./testReduced.o
	rm testReduced.o

run: main
	./main

test: test.cpp
	$(CXX) $(FLAG)  -std=c++11 test.cpp -o test.o
	./test.o

clean:
	rm *.o
