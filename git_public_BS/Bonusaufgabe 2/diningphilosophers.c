#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

//Anzahl Philosopher
#define ANZ_PHILOSOPHER (4)
// Zeit, die ein Philosopher zum Essen braucht
#define TIME_TO_EAT 10

// Mutexvariablen
pthread_mutex_t lock, safe[4];
//Philosophen ID
int neighbours [4] = { 0, 1, 2 , 3};    
// Zustand true wenn Philosoph isst, false wenn er keinen Löffel benötigt
bool requested[4];
//Wartezeit
double d_delta[8];
//Löffelzähler
size_t zaehler [4] = {0, 0, 0, 0};
// für Zufallszahlberechnung  
time_t t;
size_t z = 1;

//essen
//Keine Deadlockgefahr, da Ressourcen nach endlicher Zeit freigegeben werden und Mutex kann nur von einem Thread betreten werden
void eat (int nr){
    //WarteZeitberechnung
    struct timespec start, stop, delta;
    
    timespec_get(&start, TIME_UTC);
    //Mutex (für den Zustand des jeweiligen Philosophen) sperren
    pthread_mutex_lock(&safe[nr]); 

    timespec_get(&stop, TIME_UTC);
    delta.tv_sec = stop.tv_sec - start.tv_sec;
    delta.tv_nsec = stop.tv_nsec - start.tv_nsec;

    requested[nr] = true; 
    printf("Nr %d is eating\n", nr);    
    
      
    //Mutex freigeben
    pthread_mutex_unlock(&safe[nr]); 
    d_delta[nr] = (double) delta.tv_sec + (double) delta.tv_nsec/ 1000000000.0;

}

void finish (int nr) {
    
    //Mutex (für den Zustand des jeweiligen Philosophen) sperren
    pthread_mutex_lock(&safe[nr]);
    
    requested[nr] =false;   
    printf("Nr %d finished eating\n", nr);
    //Mutex freigeben
    pthread_mutex_unlock(&safe[nr]);
}


void think(){
    //initialize random generator
    //Fairness durch Zufalls Denkzeit -> Reihenfolge der Threads ändert sich, Rechenzeit aller Threads gleich

    srand((unsigned) time(&t));
    struct timespec delay;          //Struct für Denkzeit
    delay.tv_sec = 0;
    delay.tv_nsec = rand() / z;      //1000 ns = 1 s durch anwachsenden Zähler läuft die Nachdenkzeit gegen ein Minimum
    nanosleep(&delay,NULL); //Sleep 
    z += 10;    
    
}

void request(int nr) {
    struct timespec start, stop, delta;
    timespec_get(&start, TIME_UTC);
    
    //Nur 1 Thread zeitgleich kann die Zustände überprüfen
    //Mutex sperren
    pthread_mutex_lock(&lock); 
    //Berechnen der Wartezeit bis der krit Bereich betreten werden darf.
    timespec_get(&stop, TIME_UTC);
    delta.tv_sec = stop.tv_sec - start.tv_sec;
    delta.tv_nsec = stop.tv_nsec - start.tv_nsec;

     printf("Nr %d requested Spoon \n", nr);    
     //Überprüfung der Zustände, rechts und links
        //Fairness durch FIFO
    if( nr == 0 && requested[1] != true && requested[3] != true){ eat(nr);}
    else if( nr == 1 && requested[2] != true && requested[0] != true){ eat(nr);}
    else if( nr == 2 && requested[3] != true && requested[1] != true){ eat(nr);}
    else if( nr == 3 && requested[0] != true && requested[2] != true){ eat(nr);}
    else { printf(" requested Ressources in use\n");}
    if( (requested[0] && requested[2]) || (requested[1] && requested[3]) ) { printf("opposide Philosophers are eating at same time\n");}
     d_delta[nr+4] = (double) delta.tv_sec + (double) delta.tv_nsec/ 1000000000.0;
      
     
     //Mutex entsperren
     pthread_mutex_unlock(&lock);
    
     

       

    

}



void *task (void* args) {
 
 do{
    int* nr = args;
    printf("Nr %d is thinking\n", *nr);
    think();
    printf("Nr %d finished thinking\n", *nr);

    request(*nr);
    sleep(TIME_TO_EAT); 
    finish(*nr);
    zaehler[*nr] += 1;
    double w = d_delta[*nr] + d_delta[*nr +4];
    printf("Wartezeit von Nr %d is %f \n", *nr, w);
    printf("Nr %d hat schon %d -mal beide Löffel erhalten \n", *nr, zaehler[*nr]); 
 

 
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
