#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <time.h>

//Anzahl Philosopher
#define ANZ_PHILOSOPHER (4)
#define TIME_TO_EAT()

pthread_mutex_t lock[4];
struct neighbours { int one; int two;};    // Löffel links und Löffel rechts

void think(){
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = rand();      //1000 ns = 1 us
    nanosleep(&delay,NULL);
}
void *task (void *args) {
 struct neighbours *mySpoons = (struct neigbours  *)args;
 think();
 getSpoon(mySpoons -> one);
 getSpoon(mySpoons -> two);
 
    
    
}
void getSpoon(int nr) {
     pthread_mutex_lock(&lock[nr]); 
     struct timespec delay;
     delay.tv_sec = 0;
     delay.tv_nsec = rand();      //1000 ns = 1 us
     nanosleep(&delay,NULL);
        
     pthread_mutex_unlock(&lock[nr]);

}



int main (){
pthread_t t[ANZ_PHILOSOPHER]; 
struct neigbours p[ANZ_PHILOSOPHER];

for( counter = 0; counter < ANZ_PHILOSOPHER; counter++){
        if (pthread_mutex_init(&lock[counter], NULL)!=0){ 
        printf("\n Mutex Initialization Failed \n");
      }
    }


 for( counter = 0; counter < ANZ_PHILOSOPHER; counter++){
        
        if(counter < 3){
        p[counter] -> one = counter;
        p[counter] -> two = counter +1;
        } else {
        p[counter] -> one = counter;
        p[counter] -> two = 0;
        }
    
        pthread_create(&t[counter], NULL, &task, &p[counter]);

    }
    

for( counter = 0; counter < ANZ_PHILOSOPHER; counter++){
pthread_mutex_destroy(&lock[counter]);
}

return 0;
}
