#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef unsigned long long ull;

typedef struct {
    ull *array;
    size_t N;
} fib_arg_t;

void *fib(void *arg) {
    fib_arg_t *fa = (fib_arg_t *)arg;
    ull *arr = fa->array;
    size_t N = fa->N;
    if (N == 0) pthread_exit(NULL);
    if (N >= 1) arr[0] = 0;
    if (N >= 2) arr[1] = 1;
    for (size_t i = 2; i < N; ++i) {
        arr[i] = arr[i-1] + arr[i-2];
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }
    size_t N = (size_t)strtoull(argv[1], NULL, 10);
    ull *arr = malloc(sizeof(ull) * N);
    if (!arr) {
        perror("malloc");
        return 1;
    }

    fib_arg_t fa = { .array = arr, .N = N };
    pthread_t tid;
    if (pthread_create(&tid, NULL, fib, &fa) != 0) {
        perror("pthread_create");
        free(arr);
        return 1;
    }

    if (pthread_join(tid, NULL) != 0) {
        perror("pthread_join");
        free(arr);
        return 1;
    }

    for (size_t i = 0; i < N; ++i) {
        if (i) printf(" ");
        printf("%llu", (unsigned long long)arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
