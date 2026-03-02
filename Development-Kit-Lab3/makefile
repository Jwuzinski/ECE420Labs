SIZE = 25
BOUND = 10
THREADS = 1

CC = gcc
CFLAGS = -Wall
LIBS = -lm

# common source that must be compiled+linked into programs that use it
IO_SRC = Lab3IO.c
IO_OBJ = Lab3IO.o

all: main

main: main.o $(IO_OBJ)
	$(CC) $(CFLAGS) main.o $(IO_OBJ) -o main $(LIBS)

main.o: main.c $(IO_OBJ) timer.h
	$(CC) $(CFLAGS) -c main.c -o main.o

generate: datagen
	./datagen

datagen: datagen.o $(IO_OBJ)
	$(CC) $(CFLAGS) datagen.o $(IO_OBJ) -o datagen

datagen.o: datagen.c $(IO_OBJ) timer.h
	$(CC) $(CFLAGS) -c datagen.c -o datagen.o

$(IO_OBJ): $(IO_SRC) Lab3IO.h
	$(CC) $(CFLAGS) -c $(IO_SRC) -o $(IO_OBJ)

clean:
	rm -f *.o
	rm -f matrix_multiplier
	rm -f series
	rm -f datagen
	rm -f main
	rm -f series.txt parallel.txt
