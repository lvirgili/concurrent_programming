CC = g++
FLAGS = -std=c++0x -Wall -pthread -O4
LMATH = -lm

OBJS = ep2_io.o ep2_graph.o main.o
TARG = ep2

ep2: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARG) $(LMATH)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< $(LMATH)

relatorio.pdf: relatorio.tex
	pdflatex relatorio.tex
	pdflatex relatorio.tex

clean:
	-rm -f *.o $(TARG) *.aux *.log *~
