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

    printf("ErzeugerAddr: &p\n", shm);
    for( i = 0; i < LOOP_COUNT; i++){
        load->work = i +1;
        printf("Erzeuger: &d\n", load->result);
    
    }
//Warten bevor Clean-Up
    sleep(1);
}


void consumer (){
    void * shm
    struct sync *data;
    struct payload *load;
    int i;
    int fd;
    printf("VerbraucherAddr: &p\n", shm);
    for( i = 0; i < LOOP_COUNT; i++){
        
        printf("Verbraucher: &d\n", load->work);
    
    }
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
}
