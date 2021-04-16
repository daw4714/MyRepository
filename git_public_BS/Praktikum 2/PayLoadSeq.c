#include <stdio.h>
#include <time.h>
#include<stdbool.h>
#include<stdint.h>
#include <pthread.h>
#define MAX_TO_TEST (10)





bool  primzahl1(int  zahl){
  for( size_t i = 2; i < zahl; i++){
    if(zahl % i == 0){
      return false;
    }
   }
  return true;
 }

void *calc_prime(void *args){
int* primzahl = (int*) args;

}

void * thread_fkt(void * args){
  int* primzahl = (int*) args;
  int i =  primzahl1(*primzahl);
  return &i;
}

int main(int argc, char** argv){
 size_t counter;
 struct timespec start, stop, delta;
 timespec_get(&start, TIME_UTC);
 double d_delta;
 for(counter = 2; counter < MAX_TO_TEST; counter++){
  if( primzahl(counter)){
    printf("%d\n", counter);
  }

 }

 timespec_get(&stop, TIME_UTC);
 delta.tv_sec = stop.tv_sec - start.tv_sec;
 delta.tv_nsec = stop.tv_nsec - start.tv_nsec;
 if(start.tv_nsec > stop.tv_nsec){
   delta.tv_sec =-1;
   delta.tv_nsec += 1000000000;
 }
  d_delta = (double) delta.tv_sec + (double) delta.tv_nsec/ 1000000000.0;
  printf("%f\n", d_delta);

  return 0;
}
