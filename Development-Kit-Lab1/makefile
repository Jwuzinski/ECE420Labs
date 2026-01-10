SIZE = 25
BOUND = 10
THREADS = 1

matrix_multiplier: matrix_multiplier.c
	gcc -Wall matrix_multiplier.c -o matrix_multiplier -lpthread -lm
matrixgen: matrixgen.c
	gcc matrixgen.c -o matrixgen
series: series_multiplier.c
	gcc -Wall series_multiplier.c -o series
generate_matrix: matrixgen
	./matrixgen -s $(SIZE) -b $(BOUND)
clean:
	rm -f *.o
	rm -f matrix_multiplier
	rm -f series
	rm -f matrixgen
test_realquick:
	gcc -Wall matrix_multiplier.c -o matrix_multiplier -lpthread -lm
	./matrix_multiplier 25
	rm -f matrix_multiplier
compare_series_parallel: matrix_multiplier series
	./series $(THREADS) > series.txt
	./matrix_multiplier $(THREADS) > parallel.txt
	diff -q series.txt parallel.txt
