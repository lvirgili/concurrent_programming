CC = gcc
CFLAGS = -ansi -Wall -pedantic -lm

OBJS1=ep1_main.o ep1_ll.o ep1_io.o
PROGRAM1=ep1
TEST=ep1_test.o ep1_ll.o ep1_io.o
TPROG=test

ep1: $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $(PROGRAM1)

test: $(TEST)
	$(CC) $(CFLAGS) $(TEST) -o $(TPROG)

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS2) -c $<

clean:
	-rm -f *.o $(PROGRAM1) $(TPROG) *~
