CC = g++
FLAGS = -std=c++0x -Wall -Wfatal-errors
LMATH = -lm

OBJS = ep2_pq.o main.o
TARG = test

dagmdf: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARG) $(LMATH)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< $(LMATH)

clean:
	-rm -f *.o $(TARG) *~
