#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>

//Max Threadanzahl
#define MAX_ANZ_THREADS (20)

//Max Anzahl an Iterationen
#define MAX_CNT (100*1000*1000)

//Shared Ressource
volatile int a = 0; 
volatile int b = 0; 
volatile int c = 0;

//locks
pthread_mutex_t lock;
sem_t lock_sem;
volatile bool locked = FALSE;

//Test-And-Set-Lock Method
atomic boolean Test_And_Set_Lock( boolean *is_locked){
    boolean swaped = *is_locked;
    *is_locked = TRUE;
    return swaped;
}


 // READ-MODIFY-WRITE-Sequences
int *read_modify_write_Mutex(){
  for( size_t t = 0 ; t < MAX_CNT; t ++){
  pthread_mutex_lock(&lock); 
  a = a + 10;
  a = a-1;
  printf("%d\n ", a);

  pthread_mutex_unlock(&lock);
  }
  return a;
}
// READ-MODIFY-WRITE-Sequence
int *read_modify_write_Semaphore(){
  for( size_t t = 0 ; t < MAX_CNT; t ++){
  sem_wait(&lock_sem); 
  b = b + 10;
  b = b-1;
  printf("%d\n ", b);

  sem_post(&lock_sem);
  }
  return b;
}

// READ-Modify-Write-Sequence

int *read_modify_write_Test_And_Set_Lock(){
 for( size_t t = 0 ; t < MAX_CNT; t ++){
  
    while(Test_And_Set_Lock(&locked) == TRUE) 
    ;
        c = c + 10;
        c = c-1;
        printf("%d\n ", c);

  // Free critical Area
    locked = FALSE;  
    }
  return c;
}


int main (void * args){
size_t counter;
struct timespec start, stop, delta;
 timespec_get(&start, TIME_UTC);
 double d_delta;

//Array of Threads with length MAX_ANZ_THREADS
pthread_t t[MAX_ANZ_THREADS]; 

 //Überprüfen ob Mutex erfolgreich, sonst Fehlercode
  if (pthread_mutex_init(&lock, NULL)!=0){ 
        printf("\n Mutex Initialization Failed \n");
      }
   //Running Thread with Mutex
  for( counter = 0; counter < MAX_ANZ_THREADS; counter++){
        pthread_create(&t[counter], NULL, &read_modify_write_Mutex, NULL);

    }
  for( counter = 0; counter < MAX_ANZ_THREADS; counter++){
        pthread_join(t[counter], NULL);
    }
   pthread_mutex_destroy(&lock);
   printf("%d\n ", a);

// Analysing time
 timespec_get(&stop, TIME_UTC);
  delta.tv_sec = stop.tv_sec - start.tv_sec;
 delta.tv_nsec = stop.tv_nsec - start.tv_nsec;
 if(start.tv_nsec > stop.tv_nsec){
   delta.tv_sec =-1;
   delta.tv_nsec += 1000000000;
 }
  d_delta = (double) delta.tv_sec + (double) delta.tv_nsec/ 1000000000.0;
  printf("Mutex-Laufzeit: %f\n", d_delta);

//###########################################################################################################
struct timespec start1, stop1, delta1;
 timespec_get(&start1, TIME_UTC);
 double d_delta1;
size_t counter1;
//Array of Threads with length MAX_ANZ_THREADS
pthread_t r[MAX_ANZ_THREADS]; 

 //Überprüfen ob Mutex erfolgreich, sonst Fehlercode
  if (sem_init(&lock_sem,NULL, 1)!=0){ 
        printf("\n Semaphore Initialization Failed \n");
      }
   //Running Thread with Mutex
  for( counter1 = 0; counter1 < MAX_ANZ_THREADS; counter1++){
        pthread_create(&r[counter1], NULL, &read_modify_write_Semaphore, NULL);

    }
  for( counter1 = 0; counter1 < MAX_ANZ_THREADS; counter1++){
        pthread_join(r[counter1], NULL);
    }
   sem_destroy(&lock_sem);
   printf("%d\n ", b);

// Analysing time
timespec_get(&stop1, TIME_UTC);
  delta1.tv_sec = stop1.tv_sec - start1.tv_sec;
 delta1.tv_nsec = stop1.tv_nsec - start1.tv_nsec;
 if(start1.tv_nsec > stop1.tv_nsec){
   delta1.tv_sec =-1;
   delta1.tv_nsec += 1000000000;
 }
  d_delta1 = (double) delta1.tv_sec + (double) delta1.tv_nsec/ 1000000000.0;
  printf("Semaphore-Laufzeit: %f\n", d_delta1);

//##################################################################################################################
struct timespec start2, stop2, delta2;
 timespec_get(&start2, TIME_UTC);
 double d_delta2;

size_t counter2;
//Array of Threads with length MAX_ANZ_THREADS
pthread_t n[MAX_ANZ_THREADS]; 

 
   //Running Thread with Mutex
  for( counter2 = 0; counter2 < MAX_ANZ_THREADS; counter2++){
        pthread_create(&n[counter2], NULL, &read_modify_write_Test_And_Set_Lock, NULL);

    }
  for( counter2 = 0; counter2 < MAX_ANZ_THREADS; counter2++){
        pthread_join(n[counter2], NULL);
    }
   
   printf("%d\n ", c);

// Analysing time
timespec_get(&stop2, TIME_UTC);
  delta2.tv_sec = stop2.tv_sec - start2.tv_sec;
 delta2.tv_nsec = stop2.tv_nsec - start2.tv_nsec;
 if(start2.tv_nsec > stop2.tv_nsec){
   delta2.tv_sec =-1;
   delta2.tv_nsec += 1000000000;
 }
  d_delta2 = (double) delta2.tv_sec + (double) delta2.tv_nsec/ 1000000000.0;
  printf("Semaphore-Laufzeit: %f\n", d_delta2);


return NULL;
}
    
  

