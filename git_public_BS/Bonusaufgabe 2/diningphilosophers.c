#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

//Anzahl Philosopher
#define ANZ_PHILOSOPHER (4)
#define TIME_TO_EAT 4


pthread_mutex_t lock;
int neighbours [4] = { 0, 1, 2 , 3};    // Philosophen ID
bool requested[4];  // Zustand true wenn Philosoph isst, false wenn er keinen Löffel benötigt
time_t t;

void think(){
    //initialize random generator
    srand((unsigned) time(&t));
    struct timespec delay;          //Struct for sleeping time
    delay.tv_sec = 0;
    delay.tv_nsec = rand();      //1000 ns = 1 s
    nanosleep(&delay,NULL);     //Sleep 
}

void eat(int nr) {
     pthread_mutex_lock(&lock); //Only one at a time can check and manipulate states
    
     printf("Nr %d requested Spoon \n", nr);    // Check if Neighbours are eating, if not eat 
     if( nr == 0 && requested[1] != true && requested[4] != true){ requested[nr] = true; printf("Nr %d is eating\n", nr); sleep(TIME_TO_EAT); requested[nr] =false; printf("Nr %d finished eating\n", nr);}
     if( nr == 1 && requested[2] != true && requested[0] != true){ requested[nr] = true; printf("Nr %d is eating\n", nr); sleep(TIME_TO_EAT); requested[nr] =false; printf("Nr %d finished eating\n", nr);}
     if( nr == 2 && requested[3] != true && requested[1] != true){ requested[nr] = true; printf("Nr %d is eating\n", nr); sleep(TIME_TO_EAT); requested[nr] =false; printf("Nr %d finished eating\n", nr);}
     if( nr == 3 && requested[4] != true && requested[2] != true){ requested[nr] = true; printf("Nr %d is eating\n", nr); sleep(TIME_TO_EAT); requested[nr] =false; printf("Nr %d finished eating\n", nr);}
     if( nr == 4 && requested[0] != true && requested[3] != true){ requested[nr] = true; printf("Nr %d is eating\n", nr); sleep(TIME_TO_EAT); requested[nr] =false; printf("Nr %d finished eating\n", nr);}
     else { printf(" requested Ressources in use");}
    
      //Keine Deadlockgefahr, da Ressourcen nach endlicher Zeit freigegeben werden
     
        
     pthread_mutex_unlock(&lock);

}

void *task (void* args) {
 
 do{
 int* nr = args;
    printf("Nr %d is thinking\n", *nr);
 think();
    printf("Nr %d finished thinking\n", *nr);

 eat(*nr);
 
 }while(1);
 
    
    
}

int main (){
pthread_t t[ANZ_PHILOSOPHER]; 



        if (pthread_mutex_init(&lock, NULL)!=0){ 
        printf("\n Mutex Initialization Failed \n");
      }
    


     for( int counter = 0; counter < ANZ_PHILOSOPHER; counter++){
        
        
        pthread_create(&t[counter], NULL, &task, &neighbours[counter]);
        printf("Philosoph Nr %d sits now on the table\n", counter);

    }
    for( int counter = 0; counter < ANZ_PHILOSOPHER; counter++){
          
        pthread_join(t[counter], NULL);
        printf("Philosoph Nr %d has left the table\n", counter);

    }
    

for( int counter = 0; counter < ANZ_PHILOSOPHER; counter++){
    pthread_mutex_destroy(&lock);
}

return 0;
}
