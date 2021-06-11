struct sync {
    sem_t newWork;
    sem_t newResult;
    void* payloadOffset;
};
struct payload {
    int work;
    int result;
};
# define SHM_Name "/BS_Uebung"
# define SHM_SIZE (sizeof(struct sync) + sizeof(struct payload) + 32)
# define LOOP_COUNT 8

void producer (){
    void *shm;
    struct sync *data;
    struct payload *load;
    int i;
    int fd;
    fd = shm_open(SHM_NAME, O_RDWR | O_CREATE, S_IRUSR | S_IWUSR);
    ftruncate(fd, SHM_SIZE);
    shm = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);  //0 kein Offset, von beginn an Schreiben
    data = (struct sync *) shm;
    load = (struct payload*) data + 1;   // oder load = (struct payload*)(shm + sizeof(struct sync));
    sem_init(&data->semA, 1, 1);
    sem_init(&data->semB, 1, 0);
    data->payloadOffset = (void *) (sizeof(struct sync));

    printf("ErzeugerAddr: &p\n", shm);
    for( i = 0; i < LOOP_COUNT; i++){
        sem_wait(&data->semA);
        load->work = i +1;
        printf("Erzeuger: &d\n", load->result);
        sem_post(&data->semB);
        
    
    }
//Warten bevor Clean-Up
    sleep(1);
    
    sem_wait(&data->SemA);
    sem_destroy(&data->SemA);
    sem_destroy(&data->SemB);
    munmap(shm, SHM_SIZE);  //Mapping entfernen
    close(fd);  //File schließen
    shm_unlink(SHM_NAME);  //Entferne Name
} 


void consumer (){
    void * shm
    struct sync *data;
    struct payload *load;
    int i;
    int fd;
     fd = shm_open(SHM_NAME, O_RDWR | O_CREATE, S_IRUSR | S_IWUSR);
     shm = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);  //0 heißt kein Offset, von Beginn an Schreiben   
     data = (struct sync *) shm;
     load = (struct payload*)((uintptr_t) data->payloadOffset + (uintptr_t)shm);  

    printf("VerbraucherAddr: &p\n", shm);
    for( i = 0; i < LOOP_COUNT; i++){
        sem_wait(&data->SemB);
        printf("Verbraucher: &d\n", load->work);
        load->result = load->work *2;
        sem_post(&data->semA);
    
    }
    close(fd);
    mumap(shm, SHM_SIZE);
    //unlink(SHM_NAME);
}


int main ( int argc, char ** argv){

    int opt;
    while(( opt = getopt(argc, argv, "pc")) != -1){
        switch(opt){
            case 'p':
                producer();
                return 0;
            case 'c':
            consumer();
            return 0;
        }
    }
    printf("Folgende Optionen sind verfügber:" "\n\t-p: Starte ");
    return 0;

//Kein Thread notwendig, aufruf über Konsole
}
