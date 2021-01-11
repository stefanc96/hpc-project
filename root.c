#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int** initialize_matrix(int rows, int cols){
    int **mat = (int **)malloc(rows * sizeof(int*));

    for(int i = 0; i < rows; i++) {
        mat[i] = (int *)malloc(cols * sizeof(int));
        for(int j = 0; j < cols; j++) {
            mat[i][j] = 2;
        }
    }

    return mat;
}

void print_matrix(int** matrix, int rows, int cols){
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {
    int totalSum = 0;
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    int** matrix = initialize_matrix(rows, cols);

    print_matrix(matrix, rows, cols);

    MPI_Init(&argc, &argv);
    int rank;
    int world;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    printf("Hello: rank %d, world: %d\n",rank, world);
    MPI_Finalize();
}