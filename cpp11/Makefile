CPP = g++
FLAGS = -Wall -O2 -pthread -std=c++0x

matrix: tmatrix_multiply.cpp
	${CPP} ${FLAGS} $< -o matrix

clean:
	rm matrix *~
