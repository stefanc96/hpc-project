#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define WORKER_ID "worker"

int *initialize_matrix(int rows, int cols) {
    int *mat = (int *) malloc(rows * cols * sizeof(int));

    for (int i = 0; i < rows * cols; i++) {
        mat[i] = i;
    }

    return mat;
}

int sum_array(const int *array, int size){
    int sum = 0;

    for (int i = 0; i < size; i++) sum += array[i];

    return sum;
}

void print_matrix(int *matrix, int rows, int cols) {
    printf("Initial matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[(i * cols) + j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    int totalSum = 0;
    int rows = (int)strtol(argv[1], NULL, 10);
    int cols = (int)strtol(argv[2], NULL, 10);
    int *results = (int*) malloc(cols * sizeof(int));
    int *temp_array = (int*) malloc(rows * sizeof(int));
    char **workerArguments = (char**) malloc(4 * sizeof(char*));

    workerArguments[0] = argv[1];
    workerArguments[1] = argv[2];
    workerArguments[2] = argv[3];

    MPI_Comm workersChannel;
    int *matrix = initialize_matrix(rows, cols);

    print_matrix(matrix, rows, cols);

    MPI_Init(&argc, &argv);
    int rank;
    int world;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    MPI_Comm_spawn(WORKER_ID, workerArguments, rows, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &workersChannel, MPI_ERRCODES_IGNORE);

    MPI_Scatter(matrix, cols, MPI_INT, temp_array, cols, MPI_INT, rank == 0 ? MPI_ROOT : MPI_PROC_NULL, workersChannel);

    MPI_Gather(&totalSum, 1, MPI_INT, results, 1, MPI_INT, rank == 0 ? MPI_ROOT : MPI_PROC_NULL, workersChannel);

    totalSum = sum_array(results, cols);
    printf("Final sum: %d", totalSum);

    MPI_Comm_free(&workersChannel);

    MPI_Finalize();
}