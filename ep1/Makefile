CC = gcc
CFLAGS = -Wall -pedantic -pthread -std=c99 -D__USE_GNU
LDFLAGS = -lm

OBJS1=ep1_main.o ep1_ll.o ep1_io.o ep1_stat.o ep1_ciclista.o
PROGRAM1=ep1
TEST=ep1_test.o ep1_ll.o ep1_io.o ep1_stat.o ep1_ciclista.o
TPROG=test

ep1: $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $(PROGRAM1) $(LDFLAGS)

test: $(TEST)
	$(CC) $(CFLAGS) $(TEST) -o $(TPROG) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(LDFLAGS)

clean:
	-rm -f *.o $(PROGRAM1) $(TPROG) *~ saida.txt
