#include <stdio.h>
#include <pthread.h>
/* Usage: ./main <p> where p is the number of threads

*/

int char_to_digit(char c) {
    	// Check if the character is a digit
	if (c >= '0' && c <= '9') {
    	return c - '0'; // Convert character to integer value
	}
	return -1; // Or handle as an error if not a digit
}

void create_threads(int thread_count){
	//Thread creation
}

void open_matrix(){
	//Open matrix
}

void assign_threads(){
	//Assign threads to matrix
}

void compute_and_update(){
	//Compute the result, assign to new matrix
}

void print_matrix(){
	//Print matrix
}

int main(int argc, char *argv[]){
	//Check for number of threads
	//Error check if no threads are declared, maybe enforce a default?
	if(argc != 2){
		//Wrong number of args, decide what to do.
		return -1;
	}
	int thread_count = char_to_digit(argv[1]); //Number of threads
	if(thread_count == -1){
		//Not a digit, return error
		return -1;
	}
	//Create <p> threads

	//Open matrix

	//Assign threads to matrix

	//Compute products

	//Print results?
}
