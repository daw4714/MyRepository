#include <stdio.h>
#include <pthread.h>
#include <string.h>

struct student
{
  char name[100];
  int laenge;
};

/* Thread-Funktion */
void *example_fct(void *args){
 
  struct student *stud = (struct student *)args;
  stud->laenge = strlen((*stud).name);
  return NULL;  
}

 int  main(){
  /* Lege zwei Thread-handle an */
  pthread_t threadA;
  struct student daniel = {"Daniel", 0};
  /* Starte zwei Thread mit der auszuführenden Funktion example_fct.
     Konfigurations- und Übergabe-Parameter werden nicht gesetzt, daher NULL. */
  pthread_create(&threadA, NULL, &example_fct, &daniel);
  

  /* Warte auf Beendigung der beiden Threads */
  pthread_join(threadA, NULL);
  printf("Name: %s\n Laenge: %d\n", daniel.name, daniel.laenge);
  return 0;
}
