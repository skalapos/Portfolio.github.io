CC=gcc
CFLAGS=-std=c11

all: data task

data: data.c
	$(CC) $(CFLAGS) -o data data.c -lpthread

task: task.c
	$(CC) $(CFLAGS) -o task task.c -lpthread

