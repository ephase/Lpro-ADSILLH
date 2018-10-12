#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//define
#define THREADS 2
#define ITERATIONS 1000000

static long glob = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *tmain( void *arg){
    long tmp;
    for (int i=0; i < ITERATIONS; i++){
        if (pthread_mutex_lock(&mutex) != 0) {
            perror("Mutex lock failed");
            exit(EXIT_FAILURE);
        }
        tmp = glob;
        tmp++;
        glob = tmp;
        if (pthread_mutex_unlock(&mutex) != 0) {
            perror("Mutex lock failed");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t *t;
    void *res;
    // memory allocation for threads
    if ( (t=calloc(THREADS, sizeof(pthread_t))) == NULL) {
        perror("Unable ro allocate memory for pthread");
    }
    for (int i=0; i < THREADS; i++){
        if (pthread_create(&t[i],NULL, tmain, &"") != 0){ 
            perror("unable to create thread\n");
        }
    }
    // Need to block parent thread until brother thread are finished
    for (int j=0;j < THREADS; j++){
        // bloc main thread exec with pthread_join()
        if (pthread_join(t[j], &res) != 0){
            perror ("Unable to join process");
        }
    }
    printf("glob is %ld, should be 2000000\n", glob);
    free(t);
    exit (EXIT_SUCCESS);
}
