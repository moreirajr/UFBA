#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include <unistd.h>

//vars globais
int _espera = 5;
int _qtde_cliente = 0;

//vars semaforos
sem_t mutex;
sem_t cadeiras;
sem_t poltrona;

void *cliente(void *);

void *barbeiro(void *);

int
main(){

	pthread_t cliente_t, barbeiro_t;

	sem_init(&mutex, 0, 1);
	sem_init(&cadeiras, 0, 0);
	sem_init(&poltrona, 0, 1);

	pthread_create(&cliente_t, NULL, (void *) cliente, NULL);
	pthread_create(&barbeiro_t, NULL, (void *) barbeiro, NULL);

	pthread_join(cliente_t, NULL);
	pthread_join(barbeiro_t, NULL);

	return EXIT_SUCCESS;
}

void *barbeiro(void *m){
	while(1){
		sem_wait(&cadeiras);
		sem_wait(&mutex);
		_qtde_cliente--;
		sem_post(&mutex);
		sem_wait(&poltrona);
		fprintf(stderr, "[BARBEIRO] cortando cabelo...\n ");
		sleep(5);
		sem_post(&poltrona);
	}
	return NULL;
}

void *cliente(void *m){
	while(1){
		sem_wait(&mutex);
		if(_qtde_cliente < _espera){
			_qtde_cliente++;
			sem_post(&mutex);
			sem_post(&cadeiras);
			fprintf(stderr, "[CLIENTE] lendo...\n");
		}else{
			fprintf(stderr, "[CLIENTE] barbearia cheia...\n");
			sem_post(&mutex);
		}
		sleep(1);
	}
	return NULL;
}








