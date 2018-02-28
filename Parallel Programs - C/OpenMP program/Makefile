CC=gcc
CCTEST=gcc-7
CFLAGS=-std=c11 -fopenmp

all: a3

a3: a3.c
	$(CC) $(CFLAGS) -o a3 a3.c

a3test: a3.c
	$(CCTEST) $(CFLAGS) -o a3 a3.c

clean: 
	rm a3