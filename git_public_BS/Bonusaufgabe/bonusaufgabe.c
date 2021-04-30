#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdbool.h>

//Max Threadanzahl
#define MAX_ANZ_THREADS (70)

//Max Anzahl an Iterationen
#define MAX_CNT (100*1000*1000)

//Shared Ressource
volatile int a = 0; 
volatile int b = 0; 
volatile int c = 0;

//locks
pthread_mutex_t lock;
sem_t lock_sem;
volatile int is_locked = false;

volatile size_t t = 0;
volatile size_t s = 0;
volatile size_t v = 0;  



 // READ-MODIFY-WRITE-Sequences
void *read_modify_write_Mutex(){
  for(t ; t < MAX_CNT; t ++){
    pthread_mutex_lock(&lock); 
        a = a + 10;
        a = a-1;
        

    pthread_mutex_unlock(&lock);
  }
  return NULL;
}
// READ-MODIFY-WRITE-Sequence
void *read_modify_write_Semaphore(){
  for( s ; s < MAX_CNT; s ++){
     sem_wait(&lock_sem); 
        b = b + 10;
        b = b-1;
        

      sem_post(&lock_sem);
  }
  return NULL;
}

// READ-Modify-Write-Sequence

void *read_modify_write_Test_And_Set_Lock(){
 for( v ; v < MAX_CNT; v ++){
  
    while(__sync_lock_test_and_set(&is_locked,1)) 
    ;
        c = c + 10;
        c = c-1;
        

     // Free critical Area
        __sync_lock_release(&is_locked);  
    }
  return NULL;
}


int main (){
size_t counter;

 printf("\nLaufzeiten bei %d Threads in s: \n", MAX_ANZ_THREADS);


struct timespec start0, stop0, delta0;
timespec_get(&start0, TIME_UTC);
 double d_delta0;



for( size_t t = 0 ; t < MAX_CNT; t ++){
        c = c + 10;
        c = c-1;
}
// Analysing time
 timespec_get(&stop0, TIME_UTC);
  delta0.tv_sec = stop0.tv_sec - start0.tv_sec;
 delta0.tv_nsec = stop0.tv_nsec - start0.tv_nsec;
 if(start0.tv_nsec > stop0.tv_nsec){
    delta0.tv_sec -=1;
    delta0.tv_nsec += 1000000000;
   }
  d_delta0 = (double) delta0.tv_sec + (double) delta0.tv_nsec/ 1000000000.0;
  printf("sequentielle Ausführung-Laufzeit: %f\n", d_delta0);

//###########################################################################################################
 


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
   

// Analysing time
 timespec_get(&stop, TIME_UTC);
  delta.tv_sec = stop.tv_sec - start.tv_sec;
 delta.tv_nsec = stop.tv_nsec - start.tv_nsec;
 if(start.tv_nsec > stop.tv_nsec){
    delta.tv_sec -=1;
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
  if (sem_init(&lock_sem,0, 1)!=0){    //Zweite Variable 0 > nur innerhalb des Prozesses verwendbar
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
  

    // Analysing time
    timespec_get(&stop1, TIME_UTC);
    delta1.tv_sec = stop1.tv_sec - start1.tv_sec;
    delta1.tv_nsec = stop1.tv_nsec - start1.tv_nsec;
    if(start1.tv_nsec > stop1.tv_nsec){
        delta1.tv_sec -=1;
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
   
   

    // Analysing time
    timespec_get(&stop2, TIME_UTC);
    delta2.tv_sec = stop2.tv_sec - start2.tv_sec;
    delta2.tv_nsec = stop2.tv_nsec - start2.tv_nsec;
    if(start2.tv_nsec > stop2.tv_nsec){
         delta2.tv_sec -=1;
         delta2.tv_nsec += 1000000000;
     }
    d_delta2 = (double) delta2.tv_sec + (double) delta2.tv_nsec/ 1000000000.0;
    printf("Atomic_Test_And_Set_Lock-Laufzeit: %f\n", d_delta2);
    
   
   

    return 0;
}
    
  

