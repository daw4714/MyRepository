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


pthread_mutex_t lock, safe[4];
int neighbours [4] = { 0, 1, 2 , 3};    // Philosophen ID
bool requested[4];  // Zustand true wenn Philosoph isst, false wenn er keinen Löffel benötigt
time_t t;
size_t z = 1;

void eat (int nr){
    pthread_mutex_lock(&safe[nr]);
    requested[nr] = true; 
    printf("Nr %d is eating\n", nr);    
    sleep(TIME_TO_EAT); 
    //requested[nr] =false;   //Keine Deadlockgefahr, da Ressourcen nach endlicher Zeit freigegeben werden und Mutex kann nur von einem Thread betreten werden
    //printf("Nr %d finished eating\n", nr);
    pthread_mutex_unlock(&safe[nr]);

}

void finish (int nr) {
    pthread_mutex_lock(&safe[nr]);
    
    requested[nr] =false;   
    printf("Nr %d finished eating\n", nr);
    pthread_mutex_unlock(&safe[nr]);
}


void think(){
    //initialize random generator
    srand((unsigned) time(&t));
    struct timespec delay;          //Struct for sleeping time
    delay.tv_sec = 0;
    delay.tv_nsec = rand() / z;      //1000 ns = 1 s durch anwachsenden Zähler läuft die Nachdenkzeit gegen ein Minimum
    nanosleep(&delay,NULL); //Sleep 
    z += 10;    
    
}

void request(int nr) {
    bool can_eat[4];
    pthread_mutex_lock(&lock); //Only one at a time can check and manipulate states
    
     printf("Nr %d requested Spoon \n", nr);    // Check if Neighbours are eating, if not eat 
     
    if( nr == 0 && requested[1] != true && requested[3] != true){ can_eat[nr] = true;}
    else if( nr == 1 && requested[2] != true && requested[0] != true){ can_eat[nr] = true;}
    else if( nr == 2 && requested[3] != true && requested[1] != true){ can_eat[nr] = true;}
    else if( nr == 3 && requested[0] != true && requested[2] != true){ can_eat[nr] = true;}
    else { printf(" requested Ressources in use");}
    if( (requested[0] && requested[2]) || (requested[1] && requested[3]) ) { printf("opposide Philosophers are eating at same time\n");}
    
      
     
        
     pthread_mutex_unlock(&lock);
     

        if(can_eat[nr] == true){
             eat(nr);
             can_eat[nr] = false;
        }

    

}



void *task (void* args) {
 
 do{
    int* nr = args;
    printf("Nr %d is thinking\n", *nr);
    think();
    printf("Nr %d finished thinking\n", *nr);

    request(*nr);
    finish(*nr);
 

 
 }while(1);
 
    
    return NULL;
}

int main (){

pthread_t t[ANZ_PHILOSOPHER]; 



        if (pthread_mutex_init(&lock, NULL)!=0){ 
        printf("\n Mutex Initialization Failed \n");
      }
    


     for( int counter = 0; counter < ANZ_PHILOSOPHER; counter++){
        
        if (pthread_mutex_init(&safe[counter], NULL)!=0){ 
        printf("\n Mutex Initialization Failed \n");
      }
        pthread_create(&t[counter], NULL, &task, &neighbours[counter]);
        printf("Philosoph Nr %d sits now on the table\n", counter);

    }
    for( int counter = 0; counter < ANZ_PHILOSOPHER; counter++){
          
        pthread_join(t[counter], NULL);
        printf("Philosoph Nr %d has left the table\n", counter);

    }
    

    for( int counter = 0; counter < ANZ_PHILOSOPHER; counter++){
        pthread_mutex_destroy(&safe[counter]);
    }
    pthread_mutex_destroy(&lock);

    return 0;

}
