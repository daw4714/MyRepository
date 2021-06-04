#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define ANZ_MAX_TO_PLAY (50)
#define ANZ_TO_WIN (3)


//Schere = 0
//Stein = 1
//Papier = 2
int anzSpiele = 0;
int anzGewonnen1 = 0;
int anzGewonnen2 = 0;
int zug1 = -1;
int zug2 = -1;
sem_t sem;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t start = PTHREAD_COND_INITIALIZER;

double randInRange(int min, int max)
{
  return min + (int) (rand() / (double) (RAND_MAX) * (max - min + 1));
}
void *spiel (void *args)
{
int *id  = ( int *) args;



 while(anzSpiele <= 50){ 

////lock()
//pthread_cond_wait(&start, &mutex);
  
 sem_wait(&sem);
  
  if( zug1 == 0 && zug2 == 1 && zug1 >= 0 && zug2 >= 0){ anzGewonnen2 ++;}
  if( zug1 == 1 && zug2 == 0 && zug1 >= 0 && zug2 >= 0){ anzGewonnen1 ++;}
  if( zug1 == 2 && zug2 == 1 && zug1 >= 0 && zug2 >= 0){ anzGewonnen1 ++;}
  if( zug1 == 1 && zug2 == 2 && zug1 >= 0 && zug2 >= 0){ anzGewonnen2 ++;}
  if( zug1 == 2 && zug2 == 0 && zug1 >= 0 && zug2 >= 0){ anzGewonnen2 ++;}
  if( zug1 == 0 && zug2 == 2 && zug1 >= 0 && zug2 >= 0){ anzGewonnen1 ++;}
   if(anzGewonnen1 >=3 || anzGewonnen2 >= 3){ 
    anzSpiele = 51;
    printf("Spiel beendet");
    }
  if(*id == 1){
      zug1 = (int) randInRange(0,2);
  } else {
      zug2 = (int) randInRange(0,2);
  }

     // Semaphore oder Mutex
    anzSpiele++;
  sem_post(&sem);
  }
    
    return (void *) 0;
}


int main ()
{
pthread_t player1, player2;
int id1 = 1;
int id2= 0;
sem_init(&sem, 0,1);
pthread_create(&player1, NULL, &spiel, &id1);
pthread_create(&player2, NULL, &spiel, &id2);
pthread_join(player1, NULL);
pthread_join(player2, NULL);
sem_destroy(&sem);

return 0;
}




