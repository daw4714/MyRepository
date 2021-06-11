
struct sync {
    sem_t newWork;
    sem_t newResult;
    void* payloadOffset;
};
struct payload {
    int work;
    int result;
};


int erzeugen(){
int fd = shm_open('/sharedmemory', O_CREAT, S_IWUSR);
size_t length = ftruncate(fd, );   //??? Größe der Datei
void* memory = mmap(NULL, length, PROT_WRITE, MAP_SHARED, fd, 0); 
struct sync * struktur = (struct sync *) memory;
struct sync * struktur_anschluss = struktur +1;
struct payload * inhalt = (struct payload *) struktur_anschluss;
struct payload * inhalt_anschluss = inhalt +1;    //+1 Erhöht Adresse um Größe des Datentyps
}

int verbrauchen(){
    payload -> work = (payload -> work)*(payload->work);
}

void doFork ()
{
    pid_t p;
    p = fork();
    if( pid == 0){
        //Kind Prozess
        sleep(1);
        consumer();
    } else if ( p > 0){
        //Eltern Prozess
        producer();
    }
    else{
    printf("Fehler\n");    
    }
    
    
int main(){
    sem_init(sync->newWork, 1,1);
int id=fork();
    if(id>0)
    {
        /*parent process*/
        printf("Parent Process [Process id: %d].\n",getpid());
    }
    else if(id==0)
    {
        /*child process*/
        printf("fork created child [Process id: %d].\n",getpid());
        printf("fork parent process id: %d.\n",getppid());
    }
    else
    {
        /*fork creation faile*/
        printf("fork creation of child process failed!!!\n");
    }
 
    sem_destroy(sync-> newWork);
return 0;
}
