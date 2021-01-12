#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define WORKER_ID "worker"
#define WORKER_NUMBER 2

void show_array(int *array, int size) {
    for (int i = 0; i < size; i++) printf("%d ", array[i]);
    printf("\n");
}

int sum_array(const int *array, int size) {
    int sum = 0;

    for (int i = 0; i < size; i++) sum += array[i];

    return sum;
}


int main(int argc, char **argv) {
    int totalSum = 0;
    int rows = (int) strtol(argv[1], NULL, 10);
    int cols = (int) strtol(argv[2], NULL, 10);
    int k = (int) strtol(argv[3], NULL, 10);
    int *results = (int *) malloc(cols * sizeof(int));
    int *array = (int *) malloc(cols * sizeof(int));
    int *temp_array;
    char **workerArguments = (char **) malloc(4 * sizeof(char *));

    MPI_Comm parentChannel;
    MPI_Comm workersChannel;


    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_get_parent(&parentChannel);

    MPI_Scatter(array, cols, MPI_INT, array, cols, MPI_INT, 0, parentChannel);

    printf("Worker received array: ");
    show_array(array, cols);

    printf("Arguments: %d %d %d \n", rows, cols, k);
    if (cols <= k) {
        totalSum = sum_array(array, cols);

        printf("Worker sum: %d\n\n", totalSum);

        MPI_Gather(&totalSum, 1, MPI_INT, results, 1, MPI_INT, 0, parentChannel);
    } else {
        workerArguments[1] = (char *) malloc(4 * sizeof(char));

        workerArguments[0] = argv[1];
        sprintf(workerArguments[1], "%d", cols / WORKER_NUMBER);
        workerArguments[2] = argv[3];

        MPI_Comm_spawn(WORKER_ID, workerArguments, WORKER_NUMBER, MPI_INFO_NULL, 0, MPI_COMM_SELF, &workersChannel,
                       MPI_ERRCODES_IGNORE);

        temp_array = malloc(cols / WORKER_NUMBER * sizeof(int));
        MPI_Scatter(array, cols / WORKER_NUMBER, MPI_INT, temp_array, cols / WORKER_NUMBER, MPI_INT,
                    MPI_ROOT,
                    workersChannel);

        results = malloc(WORKER_NUMBER * sizeof(int));
        MPI_Gather(&totalSum, 1, MPI_INT, results, 1, MPI_INT, MPI_ROOT,
                   workersChannel);

        printf("Partial sum results from workers:");
        show_array(results, WORKER_NUMBER);
        totalSum = sum_array(results, WORKER_NUMBER);

        printf("Final worker sum: %d\n\n", totalSum);

        MPI_Gather(&totalSum, 1, MPI_INT, results, 1, MPI_INT, 0, parentChannel);

        MPI_Comm_free(&workersChannel);
    }


    MPI_Finalize();
}