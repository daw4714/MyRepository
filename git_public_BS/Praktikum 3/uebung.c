#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdbool.h>

//Max Threadanzahl
#define MAX_ANZ_THREADS (4)

//Max Anzahl an Iterationen
#define MAX_CNT (10000)


volatile size_t counter = 0;
pthread_mutex_t mutex;
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;

void *upcount (void *args){
    for( size_t t = 0; t < MAX_CNT; t++){
        pthread_mutex_lock(&mutex);
        counter += 1;
        pthread_mutex_unlock(&mutex);  
    }
    return NULL;
}
void *upcount2 (void *args){
    for( size_t t = 0; t < MAX_CNT; t++){
        pthread_mutex_lock(&mutex_lock);
        counter += 1;
        pthread_mutex_unlock(&mutex_lock);  
    }
    return NULL;
}
void *downcount (void *args){
    for( size_t t = 0; t < MAX_CNT; t++){
        pthread_mutex_lock(&mutex);
        counter -= 1;
        pthread_mutex_unlock(&mutex);  
    }
    return NULL;
}

int main( int argc, char **argv){
    size_t i;
    pthread_t t[MAX_ANZ_THREADS];
   if( pthread_mutex_init(&mutex, NULL) != 0){
        printf("Fehler, init");
        return -1;
    }

    for( i = 0; i < MAX_ANZ_THREADS/2; i++){

        pthread_create(&t[i], NULL, &upcount, NULL);
    }
     for( i = 0; i < MAX_ANZ_THREADS/2; i++){

        pthread_create(&t[i], NULL, &downcount, NULL);
    }
    pthread_mutex_destroy(&mutex);
    printf("%d/n", counter);
    return 0;

}
    
  

