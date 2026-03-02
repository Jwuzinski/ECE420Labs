#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "Lab3IO.h"

double **A;
double *x;
double **B; //TESTING MATRIX FOR SERIES
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
// TODO: DOUBLE CHECK
void jordan_elimination() {
    for (int k = size-1; k > 0; k--) {
        for (int row = 0; row < k; row++) {
            A[row][size] -= (A[row][k] / A[k][k]) * A[k][size];
            A[row][k] = 0;
        }
    }
}

void gaussian_elimination_series() {
    for (int k = 0; k < size; k++) {
        // PIVOTING: from row k to row n-1, find the row kp that has the maximum
        // absolute value of the element in the kth column 
        int big_row_index = k;
        double big_value = 0;

        for (int i = k; i < size; i++) {
            if (fabs(B[i][k]) > fabs(big_value)) {
                big_value = B[i][k];
                big_row_index = i;
            }
        }

        // After finding big_row swap current row with big_row
        // used size+1 to change swap the b vector as well
        double temp;
        for (int col = 0; col < size+1; col++) {
            // Swap elements at each column index
            temp = B[k][col];
            B[k][col] = B[big_row_index][col];
            B[big_row_index][col] = temp;
        }

        // ELIMINATION
        // TODO: DOUBLE CHECK
        for (int row = k+1; row < size; row++) {
            temp = B[row][k] / B[k][k];
            for (int col = k; col < size+1; col++) {
                B[row][col] -= (temp*B[k][col]);
            }
        }
    }
}

void jordan_elimination_series() {
    for (int k = size-1; k > 0; k--) {
        for (int row = 0; row < k; row++) {
            B[row][size] -= (B[row][k] / B[k][k]) * B[k][size];
            B[row][k] = 0;
        }
    }
}

int main() {
    // Load the input data
    if (Lab3LoadInput(&A, &size) != 0) {
        printf("ERROR: Couldn't load matrix");
        return 1;
    }

    // Create output vector (x)
    x = CreateVec(size);

    // TODO: TESTING REMOVE LATER
    if (Lab3LoadInput(&B, &size) != 0) {
        printf("ERROR: Couldn't load matrix");
        return 1;
    }

    // DO STUFF
    gaussian_elimination();
    jordan_elimination();

    // TODO: TESTING REMOVE LATER
    gaussian_elimination_series();
    jordan_elimination_series();

    // print result FOR TESTING
    PrintMat(A, size, size+1);

    // TESTING MATRICES A(PARALLEL) AND B(SERIES) OUTPUTS
    int mismatch_counter = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (A[row][col] != B[row][col]) {
                mismatch_counter++;
            }
        }
    }
    printf("NUMBER OF MISMATCHS: %d\n", mismatch_counter);

    // TODO: IMPLEMENT TIMER
    double time = 0.0;

    // TODO: MAKE x BY DIVIDING b BY A
    // b IS STORED IN A[][size] 
    Lab3SaveOutput(x, size, time);
}