#include <stdio.h>
#include <stdlib.h>
/* Usage: ./main <p> where p is the number of threads

*/

//Globals
int thread_count;
int matrix_size; // For some reason setting it to 0 gives me weird results
int **A = NULL; //First matrix
int **B = NULL; //Second matrix
int **C = NULL; //Product matrix

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

	//Open and read matrix
	open_matrix();

	/* After this function, the first matrix is stored in A
	   the second is stored in B, they are stored as globals
	   so implement whatever functions to use them. There's a
	   cleanup function at the end of the program
	*/
    //Compute the result, assign to new matrix
	int sum = 0;
	for (int j = 0; j < matrix_size; j++) {
        for (int i = 0; i < matrix_size; i++) {
			sum = 0;
			for (int k = 0; k < matrix_size; k++)
                sum = sum + A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }
    print_matrix(C, matrix_size);
	// print_matrix(A, matrix_size);
	// print_matrix(B, matrix_size);

	//Print results?
}
