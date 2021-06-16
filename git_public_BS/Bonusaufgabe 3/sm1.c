#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE		24

#define COUNT_PROCESS	5

#define MAX_DATA		35




struct buffer {
	u_int8_t data[BUFFER_SIZE];
	u_int8_t read;
	u_int8_t write;
};

struct sync {
	sem_t newWork;
	sem_t maxPuffer;
	sem_t reading;
	bool running;
};

	   
void erzeuger(){
	int fd = shm_open( "/share", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	struct sync* sync = (struct sync*) mmap(NULL, sizeof(struct sync) + sizeof(struct buffer) + 32, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	struct buffer* buf = (struct buffer*)  (sync + 1);
	for(int i = 0; i<MAX_DATA;i++){
		buf->data[buf->write] = i;
		buf->write++;
		if(buf->write==BUFFER_SIZE){
			buf->write = 0;
		}
		sem_post(&(sync->newWork));
		sem_wait(&(sync->maxPuffer));
	}
	sync->running = false;
	//Aufräumen
	for(int u = 0; u<COUNT_PROCESS; u++){
		sem_post(&(sync->newWork));
	}
	
	sem_destroy(&(sync->newWork));
	sem_destroy(&(sync->maxPuffer));
	sem_destroy(&(sync->reading));
	munmap((void*)sync, sizeof(struct sync) + sizeof(struct buffer) + 32);
	close(fd);
	shm_unlink("/share");
}

void verbraucher(){
	int fd = shm_open( "/share", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	struct sync* sync = (struct sync*) mmap(NULL, sizeof(struct sync) + sizeof(struct buffer) + 32, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	struct buffer* buf = (struct buffer*)(sync+1);
	while(true){
		sem_wait(&(sync->newWork));
		if(!sync->running && buf->write == buf->read)break;
		sem_wait(&(sync->reading));
		u_int8_t temp_read = buf->read;
		buf->read++;
		if(buf->read==BUFFER_SIZE){
			buf->read=0;
		}
		sem_post(&(sync->reading));
		printf("Gefundenenes Datum:\t%d von Prozess\t%d\n", buf->data[temp_read], getpid());
		sem_post(&(sync->maxPuffer));
	}
	munmap((void*)sync, sizeof(struct sync) + sizeof(struct buffer) + 32);
	shm_unlink("/share");
}

int main(int argc, char** argv){
	//Init
	int fd = shm_open( "/share", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	ftruncate(fd, sizeof(struct sync) + sizeof(struct buffer) + 32);
	struct sync* sync = (struct sync*) mmap(NULL, sizeof(struct sync) + sizeof(struct buffer) + 32, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	sem_init(&(sync->newWork),1,0);
	sem_init(&(sync->maxPuffer),1, BUFFER_SIZE-1);
	sem_init(&(sync->reading), 1, 1);
	sync->running = true;
	struct buffer* buf = (struct buffer*)  (sync + 1);
	buf->read = 0;
	buf->write = 0;
	
	
	int fork_cnt = 0;
	for(int i = 0; i<COUNT_PROCESS-1 && fork_cnt == 0; i++){
		fork_cnt += fork();
	}
	if(fork_cnt == 0)erzeuger();
	else verbraucher();
	
}
