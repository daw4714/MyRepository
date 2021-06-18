#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>

//Max Threadanzahl
#define MAX_ANZ_THREADS (20)

//Max Anzahl an Iterationen
#define MAX_CNT (100*1000*1000)

//Shared Ressource
volatile int a = 0; 
volatile int b = 0; 
volatile int c = 0;

//locks
pthread_mutex_t lock, lock2;
volatile int is_locked = false;

volatile size_t t = 0;
volatile size_t s = 0;
volatile size_t v = 0;  



 // READ-MODIFY-WRITE-Sequences
void *read_modify_write_Mutex(){
  for(t ; t < MAX_CNT; t ++){
    pthread_mutex_lock(&lock); 
    pthread_mutx_lock(&lock2);
        a = a + 10;
        a = a-1;
        

    pthread_mutex_unlock(&lock);
   pthread_mutx_unlock(&lock2);
  }
  return NULL;
}


// READ-Modify-Write-Sequence

void *read_modify_write_Test_And_Set_Lock(){
 for( v ; v < MAX_CNT; v ++){
  
     pthread_mutex_lock(&lock2);
    pthread_mutx_lock(&lock); 
        a = a + 10;
        a = a-1;
        

    pthread_mutex_unlock(&lock2);
pthread_mutx_unlock(&lock);
  }
    }
  return NULL;
}
int main (){
size_t counter;







for( size_t t = 0 ; t < MAX_CNT; t ++){
        c = c + 10;
        c = c-1;
}


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
   
   

   
    
   
   

    return 0;
}
    
  


