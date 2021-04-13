#include <stdio.h>
#include <pthread.h>

struct timespec {
  time_t tv_sec;  //Sekunden
  long   tv_nsec; //Nanosekunden
};

int primzahl1(int  zahl){

  for( size_t i = 2; i < zahl; i++){
    if(zahl % i == 0){
      return 0;
    }
   }
  return 1;
  
 }

void * thread_fkt(void * args){
  int* primzahl = (int*) args;
  int i =  primzahl1(*primzahl);
  return &i;
}

int main(){
  int istPrimzahl = 100;
  pthread_t threadA, threadB;
  for(size_t t = 0; t < istPrimzahl; t++){
      int clock_gettime(clockid_t clk_id, struct timespec *tp);
      pthread_create(&threadA, NULL, &thread_fkt, &t );
      pthread_join(&threadA, NULL);
      int clock_gettime(clockid_t clk_id, struct timespec *tp1);
  }
int clock_gettime(clockid_t clk_id, struct timespec *tp2);
  for(size_t t = 0; t < istPrimzahl/2; t++){
      
      pthread_create(&threadA, NULL, &thread_fkt, &t );  
      pthread_join(&threadA, NULL);
  }

  for(size_t t = istPrimzahl/2; t <= istPrimzahl; t++){
    
      pthread_create(&threadB, NULL, &thread_fkt, &t );
      pthread_join(&threadB, NULL);
        
 }
  int clock_gettime(clockid_t clk_id, struct timespec *tp3);
      return 0;

}


