
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>


typedef struct{
    long start;
    long end;
    int n;
} thread_arg_t;


double f(double a);
double CalcPi(int n, int T);


double f(double a){
    return (4.0/(1.0+ a*a));
}

void* thread_func( void* arg){
    thread_arg_t *t = (thread_arg_t*) arg;

    double local_sum = 0.0;
    double fH = 1.0 / (double)t->n;

    for (long i = t->start; i < t->end; i++) {
        double x = fH * ((double)i + 0.5);
        local_sum += f(x);
    }

    double *retval = malloc(sizeof(double));
    *retval = local_sum;
    return retval;
}

double CalcPi(int n, int T){

    pthread_t *threads = malloc(T * sizeof(pthread_t));
    thread_arg_t *args = malloc(T * sizeof(thread_arg_t)); 

    long base = n / T;
    long resto = n %  T;

    long inicio = 0 ;

    for (int i = 0; i < T; i++)
    {
        long tam = base + (i < resto ? 1 : 0);

        args[i].start = inicio;
        args[i].end = inicio + tam;
        args[i].n = n;

        pthread_create( &threads[i], NULL, thread_func, &args[i]);

        inicio += tam;
    }

    double total_sum = 0.0;

    for (int i = 0; i < T; i++)
    {
        void *ret;
        pthread_join(threads[i], &ret);

        double *valor = (double*) ret;
        total_sum += *valor;

        free(valor);
    }
    
    free(threads);
    free(args);


    return ((1.0 / (double)n) * total_sum);

}


int main(int argc, char const *argv[])
{
    int n = 2000000000;
    int T;

    const double fPi25DT = 3.141592653589793238462643;
    double fPi;

    if(argc < 2){
        printf("Uso: %s <num_threads>\n>", argv[0]);
        return 1;
    }

    T = atoi(argv[1]);

    if (T <= 0){
        printf("El numero de hilos deber ser positivo\n");
        return 1;
    }

    if (n <= 0 || n == 2147483647)
    {
    printf("\n El valor dado debe estar entre 0 y 2147483647\n");
    return 1;
    }
    
    fPi = CalcPi(n, T);

    printf("\n pi es aproximadamente = %.20f \nError = %.20f",
           fPi, fabs(fPi - fPi25DT));

    return 0;
}
