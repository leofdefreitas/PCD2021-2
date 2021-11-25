#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#define GENERATIONS 2000

void initializeMatrix(int *matrix, int N);
void printMatrix(int *matrix, int N);
int findIndex(int i, int j, int N);
void drawGlider(int *matrix, int N);
void drawRPentomino(int *matrix, int N);
int getNeighbors(int *matrix, int i, int j, int N);
void simulateLifeGame(int *grid, int *newGrid, int N, int gridStart, int gridEnd);
void copyMatrix(int *grid, int *newGrid, int startGrid, int endGrid, int N);
int getTotalAlive(int *grid, int N);
int getPrevious(int pos, int N);
int getNext(int pos, int N);
long long runTrial(int numThreads, int N);


void main() {
    struct timeval s1, e1;
    int N;
    int threads[] = {1, 2, 4, 8};
    printf("Starting Game of Life execution.\n");
    scanf("%d", &N);
    for (int i=0; i < 4; i++) {
        long long partialTime;
        gettimeofday(&s1, NULL);
        partialTime = runTrial(threads[i], N);
        gettimeofday(&e1, NULL);
        /* long long totalTime = (int) (1000*(e1.tv_sec - s1.tv_sec) + (e1.tv_usec - s1.tv_usec) / 1000);
        float percentage = (partialTime/totalTime)*100;
        printf("[%d thread(s)] Total time: %lld, parallel time: %lld, percentage: %.2f%\n", threads[i], totalTime, partialTime, percentage); */
    }
}

long long runTrial(int numThreads, int N) {
    omp_set_num_threads(numThreads);
    int *newGrid, *grid, totalAlive = 0;
    newGrid = malloc(sizeof(int) * N * N);
    grid = malloc(sizeof(int) * N * N);
    initializeMatrix(grid, N);
    drawGlider(grid, N);
    drawRPentomino(grid, N);
    copyMatrix(newGrid, grid, 0, N, N);
    printf("Starting execution for %d threads.\n", numThreads);
    int gridSize = N/numThreads;
    int i, threadId, gridStart, gridEnd;
    struct timeval start, end;
    long long timeMs;

    /* gettimeofday(&start, NULL);
    #pragma omp parallel default(none) private(i, threadId, gridStart, gridEnd) shared(grid, newGrid, N, gridSize)
        {
            threadId = omp_get_thread_num();
            gridStart = (threadId)*gridSize;
            gridEnd = (threadId+1)*gridSize;
            for (i=1; i <= GENERATIONS; i++) {
                simulateLifeGame(grid, newGrid, N, gridStart, gridEnd);
                #pragma omp barrier
                copyMatrix(grid, newGrid, gridStart, gridEnd, N);
                #pragma omp barrier
            }
        }
    gettimeofday(&end, NULL);
    timeMs = (int) (1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000); */
    for (int i=0; i<10; i++)
        totalAlive = getTotalAlive(grid, N);
    //printf("Time for %d threads: %lldms. Total alive = %d\n", numThreads, timeMs, totalAlive);
    return timeMs;
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

void simulateLifeGame(int *grid, int *newGrid, int N, int gridStart, int gridEnd) {
    for (int i=gridStart; i < gridEnd; i++) {
        for (int j=0; j < N; j++) {
            int neighbors = getNeighbors(grid, i, j, N);
            if ((neighbors == 2 || neighbors == 3) && grid[findIndex(i, j, N)] == 1) {
                newGrid[findIndex(i, j, N)] = 1;
            } else if (neighbors == 3 && grid[findIndex(i, j, N)] == 0) {
                newGrid[findIndex(i, j, N)] = 1;
            } else {
                newGrid[findIndex(i, j, N)] = 0;
            }
        }
    }
    return;
}

int findIndex(int i, int j, int N) {
    return i * N + j;
}

void copyMatrix(int *grid, int *newGrid, int startGrid, int endGrid, int N) {
    for (int i=startGrid; i < endGrid; i++) {
        for (int j=0; j < N; j++) {
            grid[findIndex(i, j, N)] = newGrid[findIndex(i, j, N)];
        }
    }
    return;
} 

int getTotalAlive(int *grid, int N) {
    struct timeval startTotalAlive, endTotalAlive;
    int totalAlive = 0;
    long long getAliveTime;
    gettimeofday(&startTotalAlive, NULL);
    #pragma omp parallel for
        for (int i=0; i<N*N; i++) {
            if (grid[i] == 1) {
                #pragma omp critical
                    totalAlive++;
            }
        }
    gettimeofday(&endTotalAlive, NULL);
    getAliveTime = (int) (1000000*(endTotalAlive.tv_sec - startTotalAlive.tv_sec) + (endTotalAlive.tv_usec - startTotalAlive.tv_usec));
    printf("%lld\n", getAliveTime);
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