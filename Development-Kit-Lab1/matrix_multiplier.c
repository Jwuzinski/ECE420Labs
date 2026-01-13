#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "lab1_IO.h"
#include "timer.h"
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
    //A = malloc(size*sizeof(*A));
    //B = malloc(size*sizeof(*B));
    C = malloc(size*sizeof(*C));
    //Malloc error handling needed?
    for(int i = 0; i < size; i++){
        //A[i] = malloc(size*sizeof(*A[i]));
        //B[i] = malloc(size*sizeof(*B[i]));
        C[i] = malloc(size*sizeof(*C[i]));
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
    }
    return 0; // no error
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

    // Check the constraints
    if (matrix_size % block_size != 0) {
        // n should be divisible by sqrt(p) which is block size
        printf("n should be divisible by sqrt(p).\n");
        return -1;
    }

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
    Lab1_loadinput(&A, &B, &matrix_size);
    allocate_matrices(matrix_size);

    //Create <p> threads
    double start_time;
    double end_time;
    GET_TIME(start_time);
    if (create_threads(thread_count)) {
        // ERROR Checking
        printf("ERROR: Create threads");
        return 1;
    }

    // Wait for threads to compute their block before printing the result.
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
    
    GET_TIME(end_time);
    double time = end_time - start_time;
    Lab1_saveoutput(C, &matrix_size, time);
    printf("Time taken: %f\n", time);
    
    free(thread_ranks);
    free(threads);
    thread_ranks = NULL;
    threads = NULL;
    clean_matrices(); //possibly redundent, leave at the end of program
}
