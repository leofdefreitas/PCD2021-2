#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define GENERATIONS 2000

void initializeMatrix(int *matrix, int N);
void printMatrix(int *matrix, int N);
int findIndex(int i, int j, int N);
void drawGlider(int *matrix, int N);
void drawRPentomino(int *matrix, int N);
int getNeighbors(int *matrix, int i, int j, int N);
void simulateHighLifeGame(int *grid, int *newGrid, int N);
void copyMatrix(int *grid, int *newGrid, int N);
int getTotalAlive(int *grid, int N);
int getPrevious(int pos, int N);
int getNext(int pos, int N);


void main() {
    int *newGrid, *grid, N;
    scanf("%d", &N);
    newGrid = malloc(sizeof(int) * N * N);
    grid = malloc(sizeof(int) * N * N);
    initializeMatrix(grid, N);
    drawGlider(grid, N);
    drawRPentomino(grid, N);
    copyMatrix(newGrid, grid, N);
    printMatrix(grid, N);
    printf("Initial condition: %d\n", getTotalAlive(grid, N));
    for (int i=1; i <= GENERATIONS; i++) {
        simulateHighLifeGame(grid, newGrid, N);
        if (i < 5) {
            printMatrix(grid, N);
            printf("Generation %d: %d\n", i, getTotalAlive(grid, N));
        }
    }
    printf("Last generation (%d Iterations): %d alive cells.", GENERATIONS, getTotalAlive(grid, N));
}

void printMatrix(int *matrix, int N) {
    int max = N > 50 ? 50 : N;
    for (int i=0; i < max; i++) {
        for (int j=0; j < max; j++) {
            printf("[%d]", matrix[findIndex(i, j, N)]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

void initializeMatrix(int *matrix, int N) {
    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            matrix[findIndex(i, j, N)] = 0;
        }
    }
    return;
}

void drawGlider(int *matrix, int N) {
    if (N > 2) {
        int lin = 1, col = 1;
        matrix[findIndex(lin, col+1, N)] = 1;
        matrix[findIndex(lin+1, col+2, N)] = 1;
        matrix[findIndex(lin+2, col, N)] = 1;
        matrix[findIndex(lin+2, col+1, N)] = 1;
        matrix[findIndex(lin+2, col+2, N)] = 1;
    }
    return;
}

void drawRPentomino(int *matrix, int N) {
    if (N > 30) {
        int lin = 10, col = 30;
        matrix[findIndex(lin, col+1, N)] = 1;
        matrix[findIndex(lin, col+2, N)] = 1;
        matrix[findIndex(lin+1, col, N)] = 1;
        matrix[findIndex(lin+1, col+1, N)] = 1;
        matrix[findIndex(lin+2, col+1, N)] = 1;
    }
    return;
}

int getNeighbors(int *matrix, int i, int j, int N) {
    int previousLine = getPrevious(i, N), 
        previousColumn = getPrevious(j, N),
        nextLine = getNext(i, N),
        nextColumn = getNext(j, N);
    return matrix[findIndex(previousLine, previousColumn, N)] + matrix[findIndex(previousLine, j, N)] + matrix[findIndex(previousLine, nextColumn, N)] + 
            matrix[findIndex(i, previousColumn, N)] + matrix[findIndex(i, nextColumn, N)] + 
            matrix[findIndex(nextLine, previousColumn, N)] + matrix[findIndex(nextLine, j, N)] + matrix[findIndex(nextLine, nextColumn, N)];
}

int findIndex(int i, int j, int N) {
    return i * N + j;
}

void simulateHighLifeGame(int *grid, int *newGrid, int N) {
    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            int neighbors = getNeighbors(grid, i, j, N);
            if ((neighbors == 2 || neighbors == 3) && grid[findIndex(i, j, N)] == 1) {
                newGrid[findIndex(i, j, N)] = 1;
            } else if ((neighbors == 3 || neighbors == 6) && grid[findIndex(i, j, N)] == 0) {
                newGrid[findIndex(i, j, N)] = 1;
            } else {
                newGrid[findIndex(i, j, N)] = 0;
            }
        }
    }
    copyMatrix(grid, newGrid, N);
    return;
}

void copyMatrix(int *grid, int *newGrid, int N) {
    for (int i=0; i < N*N; i++) {
        grid[i] = newGrid[i];
    }
    return;
} 

int getTotalAlive(int *grid, int N) {
    int totalAlive = 0;
    for (int i=0; i<N*N; i++) {
        if (grid[i] == 1) totalAlive++;
    }
    return totalAlive;
}

int getPrevious(int pos, int N) {
    if (pos == 0) {
        return N-1;
    } 
    return pos-1;
}

int getNext(int pos, int N) {
    if (pos == N-1) {
        return 0;
    }  
    return pos+1;
}
