#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX_ANZ_THREADS (10)
#define MAX_TO_RUN (10000)
int a = 0;
pthread_mutex_t lock;

 // READ-MODIFY-WRITE-Sequence
int *read_modify_write_Mutex(){
  for( size_t t = 0 ; t < MAX_TO_RUN; t ++){
  pthread_mutex_lock(&lock); 
  a = a + 1000;
  a = a-1;
  pthread_mutex_unlock(&lock);
  }
  return a;
}

int main (void * args){
size_t counter;
pthread_t t[MAX_ANZ_THREADS]; //Array of Threads with length MAX_ANZ_THREADS
  if (pthread_mutex_init(&lock, NULL)!=0){  //Überprüfen ob Mutex erfolgreich, sonst Fehlercode
        printf("\n Mutex Initialization Failed \n");
      }
  for( counter = 0; counter < MAX_ANZ_THREADS; counter++){
        pthread_create(&t[counter], NULL, &read_modify_write_Mutex, NULL);

    }
  for( counter = 0; counter < MAX_ANZ_THREADS; counter++){
        pthread_join(t[counter], NULL);
    }
   pthread_mutex_destroy(&lock);
   printf("%d\n ", a);

return NULL;
}
    
  

