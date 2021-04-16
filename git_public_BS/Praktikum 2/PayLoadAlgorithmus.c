#include <stdio.h>
#include<stdint.h>
#include<time.h>
#include<stdbool.h>
#include <pthread.h>
#define MAX_TO_TEST (10)

struct parameter{
  uint64_t offset;
  uint64_t step;
  uint64_t group;
};

bool  primzahl1(int  zahl){
  for( size_t i = 2; i < zahl; i++){
    if(zahl % i == 0){
      return false;
    }
   }
  return true;
 }

void *calc_prime(void *args){
 struct parameter *param = (struct parameter *) args;

  for(size_t i = 2 + param->offset; i < MAX_TO_TEST; i+= param->step){

    for(size_t j = 0; j < param->group; ++j){
      primzahl1(i+j);
    }

  }
  return NULL;

}




#define NUM_THREAD (4)
int main(void * args){

uint64_t counter;
struct parameter param[NUM_THREAD]; //Array an Structuren
pthread_t t[NUM_THREAD]; //Array an Threads
struct timespec start, stop, delta;
 timespec_get(&start, TIME_UTC);
 double d_delta;
for( counter = 0; counter < NUM_THREAD; counter ++){

  param[counter].group = 2;
  param[counter].offset = counter * param[counter].group;
  param[counter].step = 4 * param[counter].group;
  pthread_create(&t[counter], NULL, &calc_prime, &param[counter]);

}

for( counter = 0; counter < NUM_THREAD; counter ++){

  pthread_join(&t[counter], NULL);

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
return NULL;

}
