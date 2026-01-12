SIZE = 25
BOUND = 10
THREADS = 1

CC = gcc
CFLAGS = -Wall
LIBS = -lpthread -lm

# common source that must be compiled+linked into programs that use it
IO_SRC = lab1_IO.c
IO_OBJ = lab1_IO.o

all: matrix_multiplier matrixgen series

matrix_multiplier: matrix_multiplier.o $(IO_OBJ)
	$(CC) $(CFLAGS) matrix_multiplier.o $(IO_OBJ) -o matrix_multiplier $(LIBS)

matrix_multiplier.o: matrix_multiplier.c lab1_IO.h timer.h
	$(CC) $(CFLAGS) -c matrix_multiplier.c -o matrix_multiplier.o

series: series_multiplier.o $(IO_OBJ)
	$(CC) $(CFLAGS) series_multiplier.o $(IO_OBJ) -o series $(LIBS)

series_multiplier.o: series_multiplier.c lab1_IO.h timer.h
	$(CC) $(CFLAGS) -c series_multiplier.c -o series_multiplier.o

matrixgen: matrixgen.o
	$(CC) $(CFLAGS) matrixgen.o -o matrixgen

matrixgen.o: matrixgen.c
	$(CC) $(CFLAGS) -c matrixgen.c -o matrixgen.o

$(IO_OBJ): $(IO_SRC) lab1_IO.h
	$(CC) $(CFLAGS) -c $(IO_SRC) -o $(IO_OBJ)

generate_matrix: matrixgen
	./matrixgen -s $(SIZE) -b $(BOUND)

clean:
	rm -f *.o
	rm -f matrix_multiplier
	rm -f series
	rm -f matrixgen
	rm -f series.txt parallel.txt

test_realquick: matrix_multiplier
	./matrix_multiplier 4
	rm -f matrix_multiplier

compare_series_parallel: matrix_multiplier series
	./series $(THREADS) > series.txt
	./matrix_multiplier $(THREADS) > parallel.txt
	diff -q series.txt parallel.txt
