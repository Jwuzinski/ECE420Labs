#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
/* Usage: ./main <p> where p is the number of threads

*/

//Globals
int thread_count;
int block_size; //sqrt(p)
int matrix_size = 0;
int **A = NULL; //First matrix
int **B = NULL; //Second matrix
int **C = NULL; //Product matrix

int *thread_ranks;
pthread_t *threads;

void allocate_matrices(int size){
	//Size globals
	A = malloc(size*sizeof(*A));
	B = malloc(size*sizeof(*B));
	C = malloc(size*sizeof(*C));
	//Malloc error handling needed?
	for(int i = 0; i < size; i++){
		A[i] = malloc(size*sizeof(*A[i]));
		B[i] = malloc(size*sizeof(*B[i]));
		C[i] = malloc(size*sizeof(*C[i]));
	}
}

void read_matrix(FILE *fp, int **matrix, int size){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			fscanf(fp, "%d", &matrix[i][j]);
		}
	}
}

void clean_matrices(void){
	for(int i = 0; i < matrix_size; i++){
		free(A[i]);
		free(B[i]);
		free(C[i]);
	}
	free(A);
	free(B);
	free(C);
	A = NULL;
	B = NULL;
	C = NULL;
}

void open_matrix(){
        //Open matrix
        FILE *fp = fopen("data_input", "r");
        if(fp == NULL){
                perror("Failed to open file");
                return;
        }
        fscanf(fp, "%d", &matrix_size);
        printf("Matrix size is %d\n", matrix_size); //verification
        allocate_matrices(matrix_size); //allocate the matrix sizes
        read_matrix(fp, A, matrix_size); //Read first matrix
        read_matrix(fp, B, matrix_size); //Read second matrix

        fclose(fp);
}

// Thread function:
// Each thread uses this
void* assign_thread(void*arg) {
	// cast the argument back to its real type
	int rank = *(int*)arg;

    // Compute the row and column of the block assigned to the thread.
    // Check lab manual for more info.
    int x_block = rank / block_size;
    int y_block = rank % block_size;

    // Compute lower and upper bounds of the elements assigned to the thread
    int i_lower = (matrix_size / block_size) * x_block;
    int i_upper = (matrix_size / block_size) * (x_block + 1);
    int j_lower = (matrix_size / block_size) * y_block;
    int j_upper = (matrix_size / block_size) * (y_block + 1);

	// TEST
	// printf("Thread %d,   matrix_size=%d:\n", rank, matrix_size);
	// printf("Bounds (x): %d to %d\n", i_lower, i_upper);
	// printf("Bounds (y): %d to %d\n\n", j_lower, j_upper);
	// printf("BLOCKS x: %d,   y: %d\n\n", x_block, y_block);

    // Compute the block.
	int sum = 0;
	for (int j = j_lower; j < j_upper; j++) {
        for (int i = i_lower; i < i_upper; i++) {
			sum = 0;
			for (int k = 0; k < matrix_size; k++)
                sum = sum + A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }

    return NULL;
}

int create_threads(int thread_count){
    //Thread creation
    thread_ranks = (int *)malloc(sizeof(int)*thread_count);
    threads = (pthread_t *)malloc(sizeof(pthread_t)*thread_count);
    if (thread_ranks == NULL || threads == NULL) return 1; // ERROR

    for (int i = 0; i < thread_count; i++) {
		thread_ranks[i] = i;
        pthread_create(&threads[i], NULL, assign_thread, &thread_ranks[i]);
        // printf("Created thread: %d\n", &threads[i]);
    }
    return 0; // no error
}

void print_matrix(int **matrix, int size){
	//Print matrix
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			printf("%4d ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	//Check for number of threads
	//Error check if no threads are declared, maybe enforce a default?
	if(argc != 2){
		//Wrong number of args, decide what to do.
		printf("Usage: %s <p>\n", argv[0]);
		return -1;
	}
	thread_count = atoi(argv[1]); //Number of threads
    block_size = sqrt(thread_count);
	if(thread_count == 0){
		//Not a digit, return error
		return -2;
	}
	printf("There are %d available threads\n", thread_count); //verification

	//Open and read matrix
	/* After this function, the first matrix is stored in A
	   the second is stored in B, they are stored as globals
	   so implement whatever functions to use them. There's a
	   cleanup function at the end of the program
	*/
	open_matrix();

	//Create <p> threads
    if (create_threads(thread_count)) {
        // ERROR Checking
        printf("ERROR: Create threads");
        return 1;
    }

	// Wait for threads to compute their block before printing the result.
	for (int i = 0; i < thread_count; i++) {
		pthread_join(threads[i], NULL);
	}
	
	// Print Results
	print_matrix(C, matrix_size);

	// print_matrix(A, matrix_size);
	// print_matrix(B, matrix_size);

	clean_matrices(); //possibly redundent, leave at the end of program
}
