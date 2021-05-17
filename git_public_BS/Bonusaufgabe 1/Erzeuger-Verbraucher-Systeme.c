#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX_TO_RUN (100)

    typedef struct node {
        int payload;              //Payload; integer data
        struct node* next;         // Pointer to next node
    } node_t;
   pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   sem_t lock; 

    node_t* root = NULL;

    int addLL (int payload)
    {
        pthread_cond_wait(&cond, &mutex);
        node_t* newnode = (node_t*) malloc(sizeof(node_t));
        if(newnode == NULL)
        {
            return -1;
        }
   
        newnode -> payload = payload;
        newnode -> next = root;
        root = newnode;
        pthread_cond_signal(&cond);
        return 0;
    }

    int removeLL(int *payload)
    {
    sem_wait(&lock);
    node_t *last = root;
    node_t *newLast = root;
    if(root == NULL)
    {
            sem_post(&lock);
            return -1;
        }
    if(root->next == NULL)
    {
        *payload = root->payload;
        root = NULL;
        free(last);
        sem_post(&lock);
        return 0;
    }

    while(last != NULL && last->next !=NULL) //Durchlaufen bis zum letzten Element
    {
        newLast = last;
        last = last->next;
        
    }
    newLast->next = NULL;
    *payload = last->payload;
    free(last);
    sem_post(&lock);
    return 0;
    }
    
   /* int produce()
    {
        struct timespec ts;
        for(size_t = 0; i<MAX_TO_RUN; i++){
            timespec_get(&ts, TIME_UTC);
            int random = srand(ts.tv_nsec);
            addLL(random);
        }
        
    return 0;
    }

    int consume()
    {
        for(size_t = 0; i<MAX_TO_RUN; i++){
            timespec_get(&ts, TIME_UTC);
            int random = srand(ts.tv_nsec);
            removeLL(random);
        }
        return 0;
    }
*/     

           
        
    

    int main (){

        sem_init(&lock,0,0);

        int i = 0;
        addLL(5);
        addLL(0);
        addLL(2);
        removeLL(&i);
        while(removeLL(&i) == 0)
        printf("Removing...");
        
    
        pthread_cond_destroy(&cond);
        sem_destroy(&lock);
        return 0;
    }
