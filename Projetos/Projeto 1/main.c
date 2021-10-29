#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void initializeMatrix(int **matrix, int N) {
    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            matrix[i][j] = 0;
        }
    }
    return;
}

void printMatrix(int **matrix, int N) {
    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            printf("%d", matrix[i][j]);
        }
        printf("\n");
    }
    return;
}

void main() {
    int *newGrid, *grid, N;
    scanf("%d", &N);
    newGrid, grid = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++)
        newGrid[i], grid[i] = (int*)malloc(N * sizeof(int));

    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            newGrid[i][j] = 0;
            printf("%d", newGrid[i][j]);
        }
        printf("\n");
    }
    /* initializeMatrix(newGrid, N);
    printMatrix(newGrid, N); */

}