#define LAB4_EXTEND

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "Lab4_IO.h"
#include "timer.h"
#include "Lab4_IO.h"

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

//Need to handle scenario when node count is not
//divisible by the number of processes perfectly.
void build_displs(int n, int p, int *counts, int *displs){
        int base = n / p; //Base of perfect division
        int rem = n % p;
        int offset = 0;
        for(int rank = 0; rank < p; rank++){
                counts[rank] = base + (rank < rem ? 1 : 0); //first rem ranks get extra node.
                displs[rank] = offset;
                offset += counts[rank];
        }
}

int main (int argc, char* argv[]){
    // instantiate variables
    struct node *nodehead;
    int nodecount;
    double *r, *r_pre;
    int i, j;
    //int iterationcount;
    double start, end;
    FILE *ip;
    /* INSTANTIATE MORE VARIABLES IF NECESSARY */
    int my_rank; //PID
    int comm_size; //# MPI processes
    int ln, s_idx, e_idx; //Piece of graph owned per process
    int *counts = NULL;
    int *displs = NULL;
    int *outdeg = NULL;
    int *loutdeg = NULL;
    double lnum, lden, gnum, gden, err;
    double *lr;

    //MPI setup
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    // load data
    if(my_rank == 0){ //Only let 1 thread open data.
        if ((ip = fopen("data_input_meta","r")) == NULL) {
                printf("Error opening the data_input_meta file.\n");
                MPI_Abort(MPI_COMM_WORLD, 253);
        }
        fscanf(ip, "%d\n", &nodecount);
        fclose(ip);
    }

    MPI_Bcast(&nodecount, 1, MPI_INT, 0, MPI_COMM_WORLD); //let rank 0 send nodecount to every other process

    //Build dis
    counts = malloc(comm_size * sizeof(int));
    displs = malloc(comm_size * sizeof(int));
    build_displs(nodecount, comm_size, counts, displs);

    ln = counts[my_rank]; //how many nodes this rank owns
    s_idx = displs[my_rank]; //where the nodes rank starts in the full graph
    e_idx = s_idx + ln; //where it ends

    //Load part of graph
    if (node_init(&nodehead, s_idx, e_idx)){
        MPI_Abort(MPI_COMM_WORLD, 254);
    }

    // initialize variables
    r = malloc(nodecount * sizeof(double));
    r_pre = malloc(nodecount * sizeof(double));
    lr = malloc(ln * sizeof(double));
    outdeg = malloc(nodecount * sizeof(int)); //hold out degrees for all nodes
    loutdeg = malloc(ln * sizeof(int)); //hold out degrees for ranks own nodes

    //Initialize PageRank to uniform values, equation 1
    for (i = 0; i < nodecount; ++i) {
        r[i] = 1.0 / nodecount;
        r_pre[i] = 1.0 / nodecount;
    }

    //build local out degree
    for(i = 0; i < ln; i++){
        loutdeg[i] = nodehead[i].num_out_links;
    }

    //Gather all out degrees to every process, L_j
    //Every node should know the out degree of every node
    MPI_Allgatherv(loutdeg, ln, MPI_INT, outdeg, counts, displs, MPI_INT, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD); //synchronize

    GET_TIME(start);
    do{
        //copy r into r_pre so r_pre holds the old iteration values
        vec_cp(r, r_pre, nodecount);
        lnum = 0.0;
        lden = 0.0;

        //loop over ranks local nodes
        for(i = 0; i < ln; i++){
                int global_i = s_idx + i; //global node
                double sum = 0.0;
                for(j = 0; j < nodehead[i].num_in_links; j++){
                        int src = nodehead[i].inlinks[j] - 1; //get source ID
                        sum += r_pre[src] / outdeg[src]; //equation 2
                }
                lr[i] = (1.0 - DAMPING_FACTOR) / nodecount + DAMPING_FACTOR * sum; //equation 3
                lnum += (lr[i] - r_pre[global_i]) * (lr[i] - r_pre[global_i]); //add squared difference, intermediate for equation 4
                lden += r_pre[global_i] * r_pre[global_i]; //add square of r(t), intermediate for equation 4
        }

        //Gather local r to r for all processes so that they are all the same.
        MPI_Allgatherv(lr, ln, MPI_DOUBLE, r, counts, displs, MPI_DOUBLE, MPI_COMM_WORLD);

        //sum local numerator/denominator to global num/den
        MPI_Allreduce(&lnum, &gnum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(&lden, &gden, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        err = sqrt(gnum) / sqrt(gden); //equation 4, check above equation for lnum and lden if confusing.

    } while(err >= EPSILON);

    GET_TIME(end);
    Lab4_saveoutput(r, nodecount, end - start);

    // post processing
    node_destroy(nodehead, nodecount);
    free(r);
    free(r_pre);
    free(lr);
    free(outdeg);
    free(loutdeg);
    free(counts);
    free(displs);

    MPI_Finalize();
    return 0;
}
