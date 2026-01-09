#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
/* Usage: ./main <p> where p is the number of threads

*/

//Globals
int thread_count;
int matrix_size = 0;
int **A = NULL; //First matrix
int **B = NULL; //Second matrix

void create_threads(int thread_count){
	//Thread creation
}

void allocate_matrices(int size){
	//Size globals
	A = malloc(size*sizeof(*A));
	B = malloc(size*sizeof(*B));
	//Malloc error handling needed?
	for(int i = 0; i < size; i++){
		A[i] = malloc(size*sizeof(*A[i]));
		B[i] = malloc(size*sizeof(*B[i]));
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
	}
	free(A);
	free(B);
	A = NULL;
	B = NULL;
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

void assign_threads(){
	//Assign threads to matrix
}

void compute_and_update(){
	//Compute the result, assign to new matrix
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
	if(thread_count == 0){
		//Not a digit, return error
		return -2;
	}
	printf("There are %d available threads\n", thread_count); //verification
	//Create <p> threads

	//Open and read matrix
	open_matrix();

	/* After this function, the first matrix is stored in A
	   the second is stored in B, they are stored as globals
	   so implement whatever functions to use them. There's a
	   cleanup function at the end of the program
	*/
	print_matrix(A, matrix_size);
	print_matrix(B, matrix_size);

	//Assign threads to matrix

	//Compute products

	//Print results?


	clean_matrices(); //possibly redundent, leave at the end of program
}
