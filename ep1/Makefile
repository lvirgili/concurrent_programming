CC = gcc
CFLAGS = -Wall -pedantic -std=c99
LDFLAGS = -lm

OBJS1=ep1_main.o ep1_ll.o ep1_io.o ep1_stat.o
PROGRAM1=ep1
TEST=ep1_test.o ep1_ll.o ep1_io.o ep1_stat.o
TPROG=test

ep1: $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $(PROGRAM1) $(LDFLAGS)

test: $(TEST)
	$(CC) $(CFLAGS) $(TEST) -o $(TPROG) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(LDFLAGS)

clean:
	-rm -f *.o $(PROGRAM1) $(TPROG) *~
