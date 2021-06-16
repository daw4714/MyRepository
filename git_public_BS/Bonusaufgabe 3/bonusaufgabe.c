#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>




#define Ringpuffergroesse	24    //Größe der Speicherstruktur

#define Anzahl_Prozesse	5      // Anzahl der zu erzeugenden Prozesse

#define MAX_DATA		35   //Anzahl an Einträgen



//Ringpufferstruktur , erstes Element mit Index 0
struct ringpuffer {
	int data[Ringpuffergroesse]; //Inhalt
	int read;  //Zähler für das aktuell zu lesende Element
	int write; //Zähler für das aktuell hinzuzufügende Element
};

struct sync {
    //Semaphoren für Koordination von Verbraucher und Erzeuger
	sem_t verbraucher;    
	sem_t erzeuger;

	sem_t reading;      // Semaphore für Lesezeiger
	bool running;   // Zustand von Erzeuger Prozess, false wenn beendet
};

	   
void erzeuger(){
    //Anlegen eines Pointers auf den Shared Memory im Erzeuger
	int fd = shm_open( "/share", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	struct sync* sync = (struct sync*) mmap(NULL, sizeof(struct sync) + sizeof(struct ringpuffer) + 32, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	struct ringpuffer* ringpuffer = (struct ringpuffer*)  (sync + 1);
    
    //Erzeugen von Einträgen im Ringpuffer
	for(int i = 0; i<MAX_DATA;i++){   //Schleife über max Anzahl an Einträgen
		ringpuffer->data[ringpuffer->write] = i;
		ringpuffer->write++;
		if(ringpuffer->write==Ringpuffergroesse){  // Wenn letztes Element erreicht, Rücksprung des Schreib-Zeigers auf das erste Element.
			ringpuffer->write = 0;  
		}
		sem_post(&(sync->verbraucher));         //Verbraucherprozesse freigeben und Erzeugerprozess sperren
		sem_wait(&(sync->erzeuger));
	}
	
    sync->running = false;  //Erzeuger terminiert

	//Aufräumen
	for(int i = 0; i<Anzahl_Prozesse; i++){        // Schlussendlich alle Verbraucher freigeben und Arbeit beenden lassen
		sem_post(&(sync->verbraucher));
	}
	 
    //Auflösen der Semaphoren
	sem_destroy(&(sync->verbraucher));
	sem_destroy(&(sync->erzeuger));
	sem_destroy(&(sync->reading));
    //Freigeben des Shared Memory
	munmap((void*)sync, sizeof(struct sync) + sizeof(struct ringpuffer) + 32);
	close(fd);
	shm_unlink("/share");
}

void verbraucher(){
    //Anlegen eines Pointers für den Zugriff auf den Shared Memory im Verbraucher
	int fd = shm_open( "/share", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	struct sync* sync = (struct sync*) mmap(NULL, sizeof(struct sync) + sizeof(struct ringpuffer) + 32, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	struct ringpuffer* ringpuffer= (struct ringpuffer*)(sync+1);
    //Solange Erzeuger arbeitet und Schreibzeiger entspricht Lesezeiger -> Nur ein Durchlauf durch den Ringpuffer, keine doppelte Bearbeitung
	while(true){
		sem_wait(&(sync->verbraucher));
        //Solange Erzeuger arbeitet und Schreibzeiger entspricht Lesezeiger -> Nur ein Durchlauf durch den Ringpuffer, keine doppelte Bearbeitung
		if(!sync->running && ringpuffer->write == ringpuffer->read)break;
        //Sperren des Lesezeigers, anschließend erhöhen um eins. Zeigt auf das neue Element
		sem_wait(&(sync->reading));
		int last_read = ringpuffer->read;
		ringpuffer->read++;
		if(ringpuffer->read==Ringpuffergroesse){  // Zurücksetzen des Lesezeigers auf erstes Element, wenn Ringpuffer vollends durchlaufen
			ringpuffer->read=0;
		}
		sem_post(&(sync->reading));
		printf("Gefundenener Eintrag:\t%d von Prozess mit Nummer \t%d\n", ringpuffer->data[last_read], getpid());
		sem_post(&(sync->erzeuger));
	}
	munmap((void*)sync, sizeof(struct sync) + sizeof(struct ringpuffer) + 32);
	shm_unlink("/share");
}

int main(){
	//Anfangsinitialisierung des Shared Memory
	int fd = shm_open( "/share", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	ftruncate(fd, sizeof(struct sync) + sizeof(struct ringpuffer) + 32);
	struct sync* sync = (struct sync*) mmap(NULL, sizeof(struct sync) + sizeof(struct ringpuffer) + 32, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    // Semaphoren initialisieren
	sem_init(&(sync->verbraucher),1,0);
	sem_init(&(sync->erzeuger),1, Ringpuffergroesse-1);
	sem_init(&(sync->reading), 1, 1);
    // Anfangs ist der Verbraucherzustand true
	sync->running = true;
    // Payload ist in der Folgeadresse von Sync gespeichert
	struct ringpuffer* ringpuffer = (struct ringpuffer*)  (sync + 1);
    // Zeiger auf aktuelle Schreib- und Leseposition anfangs = 0
	ringpuffer->read = 0;
	ringpuffer->write = 0;
	
	//dynamisches Anlegen der Prozesse
	int pid = 0;
	for(int i = 0; i<Anzahl_Prozesse-1 && pid == 0; i++){
		pid= fork();  
	}
	if(pid == 0)erzeuger();  //KindProzess
	else verbraucher();     //ElternProzesse
return 0;
	
}
