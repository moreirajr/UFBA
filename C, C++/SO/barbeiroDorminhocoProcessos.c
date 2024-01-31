#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

//vars globais
int _espera = 5;
int *_qtde_cliente;

//vars semaforos
sem_t *mutex;
sem_t *cadeiras;
sem_t *poltrona;

void *cliente(void *);

void *barbeiro(void *);

int main(){
	long pid1, pid2;
	int status, shmID_2;
	key_t key_2 = 567193;
	
	
	_qtde_cliente= (int*) malloc (sizeof(int));
	
	shmID_2 = shmget(key_2, sizeof(int), IPC_CREAT | 0666);
	_qtde_cliente = shmat(shmID_2, NULL, 0);
	
	*_qtde_cliente= 0;
	
	/* place semaphore in shared memory */
	mutex	 = mmap(NULL,sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    cadeiras = mmap(NULL,sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    poltrona = mmap(NULL,sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	
	if( (!mutex) || (!cadeiras) || (!poltrona) )
		perror("mmap error");
		
		
		
		
	
	sem_init(mutex, 1, 1);
	sem_init(cadeiras, 1, 0);
	sem_init(poltrona, 1, 1);
	
	
	
	pid1 = fork();
	if(pid1 != 0){
		pid2 = fork();
		wait(&status);
	}
		
		
		
	if(pid1 == 0){
		barbeiro(NULL);
	}
	
	
	if(pid2 == 0){
		cliente(NULL);
	}
	
	
	
	
	if (sem_destroy(mutex) < 0) {
		perror("sem_destroy failed");
		exit(0);
	}
 
  /* don't forget to unmap the memory */
	if (munmap(mutex, sizeof(sem_t)) < 0) {
		perror("munmap failed");
		exit(0);
	}
	
	sem_destroy(poltrona);
	munmap(poltrona, sizeof(sem_t));
	sem_destroy(cadeiras);
	munmap(cadeiras, sizeof(sem_t));

	
	return EXIT_SUCCESS;
}

void *barbeiro(void *m){
	while(1){
		sem_wait(cadeiras);
		sem_wait(mutex);
		(*_qtde_cliente)--;
		sem_post(mutex);
		sem_wait(poltrona);
		fprintf(stderr, "[BARBEIRO] cortando cabelo...\n ");
		sleep(5);
		sem_post(poltrona);
	}
	return NULL;
}

void *cliente(void *m){
	while(1){
		sem_wait(mutex);
		if( (*_qtde_cliente) < _espera){
			(*_qtde_cliente)++;
			sem_post(mutex);
			sem_post(cadeiras);
			fprintf(stderr, "[CLIENTE] lendo...\n");
		}else{
			fprintf(stderr, "[CLIENTE] barbearia cheia...\n");
			sem_post(mutex);
		}
		sleep(1);
	}
	return NULL;
}








