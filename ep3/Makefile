CC = g++
FLAGS = -std=c++0x -Wall -pthread -g
LMATH = -lm

OBJS = ep3_passageiro.o ep3_monitor.o ep3_main.o
TARG = ep3

ep3: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARG) $(LMATH)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< $(LMATH)

clean:
	-rm -f *.o $(TARG) *.aux *.log *~
