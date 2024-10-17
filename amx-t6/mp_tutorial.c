#include <stdio.h>
#include <omp.h>

int main() {
    int i;
    int n = 10;
    int sum = 0;

#pragma omp parallel for reduction(+:sum)
    for (i = 0; i < n; i++) {
        sum += i;
        printf("Thread %d processes i = %d\n", omp_get_thread_num(), i);
    }

    printf("Total sum = %d\n", sum);
    return 0;
}
