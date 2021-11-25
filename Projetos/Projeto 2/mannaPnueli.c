#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 500000

int criticalRegionDemonstration() {
    int threadId, threadQty;
    int threadsNotFinished, i, sum = 0;
    int request, respond;

    #pragma omp parallel private(threadId, i) shared(sum) 
    {
        threadQty = omp_get_num_threads();
        threadsNotFinished = threadQty;
        threadId = omp_get_thread_num();
        if (threadId == 0) { //server
            while (threadsNotFinished > 1) {
                while (request == 0 && threadsNotFinished > 1) {}
                respond = request;
                while (respond != 0 && threadsNotFinished > 1) {}
                request = 0;
            }
        } else { //client
            for (i = 0; i < N; i++) {
                //Critical Region Access Control
                while (respond != threadId) {
                    request = threadId;
                }

                // BEGIN of Critical Region
                int localFactor = sum;
                sleep(rand()%2);
                sum = localFactor + 1;
                printf("Thread %d on its iteration number %d. Current sum value is %d\n", threadId, i, sum);
                // END end Critical Region
                respond = 0;
            }
        }
        threadsNotFinished--;
    }
    return sum;
}

int main() {
    int result;
    omp_set_num_threads(5);
    result = criticalRegionDemonstration();
    printf("Sum = %d\n", result);
    return 0;
}