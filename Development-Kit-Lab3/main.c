#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "Lab3IO.h"

double **A;
int size;

// Algorithm 1 of lab manual
void gaussian_elimination() {
    for (int k = 0; k < size; k++) {
        // PIVOTING: from row k to row n-1, find the row kp that has the maximum
        // absolute value of the element in the kth column 
        int big_row_index = k;
        double big_value = 0;

        for (int i = k; i < size; i++) {
            if (fabs(A[i][k]) > fabs(big_value)) {
                big_value = A[i][k];
                big_row_index = i;
            }
        }

        // After finding big_row swap current row with big_row
        // used size+1 to change swap the b vector as well
        double temp;
        for (int col = 0; col < size+1; col++) {
            // Swap elements at each column index
            temp = A[k][col];
            A[k][col] = A[big_row_index][col];
            A[big_row_index][col] = temp;
        }

        // ELIMINATION
        // TODO: DOUBLE CHECK
        for (int row = k+1; row < size; row++) {
            temp = A[row][k] / A[k][k];
            for (int col = k; col < size+1; col++) {
                A[row][col] -= (temp*A[k][col]);
            }
        }
    }
}

// Algorithm 2 of lab manual
void jordan_elimination() {

}

int main() {
    // Load the input data
    if (Lab3LoadInput(&A, &size) != 0) {
        printf("ERROR: Couldn't load matrix");
        return 1;
    }

    // DO STUFF
    gaussian_elimination();
    jordan_elimination();

    // print result
    PrintMat(A, size, size+1);
}