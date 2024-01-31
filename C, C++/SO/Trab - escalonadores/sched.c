/* Universidade Federal da Bahia - UFBA
 * Alunos: Evaldo Moreira, Lucas Borges, Thiago Pacheco
 * 
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/mman.h>


#define QUANTUM 1				//unidade de tempo de tarefa, 1 segundo de execucao


typedef struct t{
	int dados[5];
	/*
	 	int id;					indice 0
		int instante_chegada;	...
		int tempo_execucao;		...
		int inicio_IO;			...
		int termino_IO;			indice 4
	 */
	struct t *next;
}Tarefa;


FILE *input, *output;
Tarefa *tarefas_input, *tarefas_output, *lista_tarefas, *tarefa;	//tarefa =  compartilhada
int *instante,  n_processos;


sem_t *mutex;
sem_t *mutex2; 


void FirstComeFirstServed();
void RoundRobin();
void ShortestJobFirst();
void lerEntradasArquivo();
int tamanho_alloc();
void getTarefas(int n);
Tarefa *criar_tarefas(Tarefa *t, int n);
Tarefa *alocarTarefa();
void fprinTest(Tarefa *t);
float tempo_medio(Tarefa *t);
void menor_tempo();



int main(int argc, char *argv[]) {
	int tempo_total = 0, shmID_1, shmID_2, status;
	long pid_filho1, pid_filho2;
	key_t key_1 = 567194, key_2 = 567195;

	
	input = fopen(argv[2], "r");
	n_processos = tamanho_alloc();			//numero de processos a serem simulados
	fclose(input);
		
	
	//compartilhar tarefa e instante entre os processos
	instante = (int*) malloc (sizeof(int));
	shmID_2 = shmget(key_2, sizeof(int), IPC_CREAT | 0666);
	instante = shmat(shmID_2, NULL, 0);
	*instante = 0;
	
	tarefas_input = criar_tarefas(tarefas_input, n_processos);		//armazena entrada do arquivo

	tarefa = (Tarefa*) malloc(sizeof(Tarefa));
	if ( (shmID_1 = shmget(key_1, sizeof(Tarefa), IPC_CREAT | 0666)) < 0 ){
		perror("shmget error");
		exit(1);
	}
	if( (tarefa = shmat(shmID_1, NULL, 0)) == (Tarefa*) -1){
		perror("shmat error");
		exit(1);
	}
	
	//
	tarefa->dados[2] = 0;
	
	
	mutex  = mmap(NULL,sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	mutex2 = mmap(NULL,sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	
	if(!mutex || !mutex2)
		perror("mmap error");
		
		
	//iniciados em 0 e 1
	sem_init(mutex, 1, 0);
	sem_init(mutex2, 1, 1);
	
	
	pid_filho2 = fork();
	if(pid_filho2 != 0){
		pid_filho1 = fork();
		wait(&status);
	}
	
	
	//processo CPU
	 if(pid_filho1 == 0){
		 int i;
		 tarefas_output = (Tarefa*) calloc(n_processos, sizeof(Tarefa));
		
		 for(i=0; i < n_processos; i++){
			tarefas_output[i].dados[1] = -1;
		}
		
		 while(1){
			 sem_wait(mutex);
			 
			 if(tarefa->dados[0] == -1){//fprintf(stderr, "Tarefa null no instante %d\n", *instante);
				break;
			}//fprintf(stderr, "Tarefa {%d;%d;%d;%d,%d} no instante %d\n", tarefa->dados[0], tarefa->dados[1], tarefa->dados[2], tarefa->dados[3], tarefa->dados[4], *instante);
			else if(tarefa->dados[0] == 0){
				fprintf(stderr, "CPU idle...\n");
			}
			else{
			 
				 fprintf(stderr, "Executando Tarefa %d...\n", tarefa->dados[0]);
				 sleep(QUANTUM);
				 tarefa->dados[2] -= QUANTUM;		//tempo restante de execucao
				
				 tempo_total = (*instante);
				
				 if( tarefas_output[ tarefa->dados[0] -1].dados[1] == -1){
					 tarefas_output[ tarefa->dados[0] -1].dados[1] = (*instante);
					 tarefas_output[ tarefa->dados[0] -1].dados[0] = tarefa->dados[0];
				 }
				 else
					tarefas_output[ tarefa->dados[0] -1].dados[2] = (*instante);
			 }
		
			 sem_post(mutex2);
			  
		 }
		 
		 //salvar saida
		 output = fopen(argv[3], "w");
		 for(i=0; i < n_processos; i++)
			fprintf(output, "%d;%d;%d;\n", tarefas_output[i].dados[0], tarefas_output[i].dados[1], tarefas_output[i].dados[2]);
		 fprintf(output, "Tempo Total: %d\nTempo medio: %.2f\n", tempo_total, tempo_medio(tarefas_output));
		 fclose(output);
		 exit(0);
		 
	 }
	 
	 
	 //processo escalonador
	 if(pid_filho2 == 0){
		 input = fopen(argv[2], "r");
		 lerEntradasArquivo();
		 //fprinTest(tarefas_input);
					 
		 if(!strcmp(argv[1], "FCFS"))
			 FirstComeFirstServed();
		 else if(!strcmp(argv[1], "RR"))
			 RoundRobin();
		 else if(!strcmp(argv[1], "SJF"))
			 ShortestJobFirst();

		 fclose(input);
		 exit(0);
	 }
	 
	 
	sem_destroy(mutex);
	munmap(mutex, sizeof(sem_t));
	sem_destroy(mutex2);
	munmap(mutex2, sizeof(sem_t));
	
	return EXIT_SUCCESS;
	
}




/* ########################################################## */
/* ########################################################## */
/* 							FUNCOES							 */
/* ########################################################## */




void FirstComeFirstServed(){
	int escalonando = 1;
	Tarefa *aux;
	
	
	while(escalonando){
		sem_wait(mutex2);
		getTarefas(*instante);
		*tarefa = *lista_tarefas;
		
		//fprintf(stderr, "Lista = ");fprinTest(lista_tarefas);
		
		sem_post(mutex);
		sem_wait(mutex2);
		
		*lista_tarefas = *tarefa;	
		if( (tarefa->dados[2]==0) && (tarefa->dados[0]!=0) ){ //terminou de executar
			aux = lista_tarefas;
			lista_tarefas = lista_tarefas->next;
			free(aux);
		}
				
		(*instante)++;
		
		if(lista_tarefas==NULL){
			escalonando = 0;
			tarefa->dados[0] = -1;
			sem_post(mutex);
		}
		sem_post(mutex2);
		
		
	}
}

/* ########################################################## */

void RoundRobin(){
	int escalonando = 1;
	Tarefa *aux, *aux2;

	
	while(escalonando){
		sem_wait(mutex2);
		getTarefas(*instante);
		*tarefa = *lista_tarefas;

		sem_post(mutex);
		//esperar execucao
		sem_wait(mutex2);
		
		*lista_tarefas = *tarefa;
				
		if( (tarefa->dados[2]==0) && (tarefa->dados[0]!=0) ){		//tempo de execucao = 0
			aux = lista_tarefas;
			lista_tarefas = lista_tarefas->next;
			free(aux);
			 
		}
		else
		{	//colocar tarefa executada no final da fila
			aux = lista_tarefas;
			while(aux->next!=NULL)
				aux = aux->next;
				
			aux->next = lista_tarefas;
			aux2 = lista_tarefas;
			lista_tarefas = lista_tarefas->next;
			aux2->next = NULL;
			//fprinTest(lista_tarefas);
			
			}		
	
			(*instante)++;
			
			if(lista_tarefas==NULL){
				escalonando = 0;
				tarefa->dados[0] = -1;
				sem_post(mutex);
			}

			sem_post(mutex2);
		}

}

/* ########################################################## */

void ShortestJobFirst(){
	int escalonando = 1, mControl = 1;
	Tarefa *aux;
	
	
	while(escalonando){
		sem_wait(mutex2);
		getTarefas(*instante);
		if(mControl)	menor_tempo();
		*tarefa = *lista_tarefas;
		
		//fprintf(stderr, "Lista = ");fprinTest(lista_tarefas);
		
		sem_post(mutex);
		sem_wait(mutex2);
		
		*lista_tarefas = *tarefa;	
		if( (tarefa->dados[2]==0) && (tarefa->dados[0]!=0) ){ //terminou de executar
			aux = lista_tarefas;
			lista_tarefas = lista_tarefas->next;
			free(aux);
			mControl = 1;
		}
				
		(*instante)++;
		
		if(lista_tarefas==NULL){
			escalonando = 0;
			tarefa->dados[0] = -1;
			sem_post(mutex);
		}
		sem_post(mutex2);
			
	}

}

/* ########################################################## */

void lerEntradasArquivo(){
	char c, temp[100];
	int i = 0, j = 0;
	Tarefa *aux = tarefas_input;

	rewind(input);


	while( (c = fgetc(input) ) != EOF){
		if(c == '\n'){
			aux = aux->next;
			i=0;
			j=0;
		}
		else
		{	if(isdigit(c)){
				temp[j++] = c;
				temp[j] = '\0';

			}
			else
			{
				aux->dados[i++] = atoi(temp);
				memset(temp, 0, 100);
				j=0;
			}// ( c == ; )
		}// (c != \n)
	}//end while
}

/* ########################################################## */

void getTarefas(int n){
	Tarefa *input_aux = tarefas_input,
		   *aux2 = lista_tarefas,
		   *ant = NULL;

	
	if(lista_tarefas==NULL){
		lista_tarefas = criar_tarefas(lista_tarefas,1);
		aux2 = lista_tarefas;
	}
	else if(n <= n_processos){
		aux2 = lista_tarefas;
		while( (aux2!=NULL) && (aux2->dados[0]!=0) ){
			ant = aux2;
			aux2 = aux2->next;
		}
	}
	else return;
	
	
	while(input_aux!=NULL){
		//instante de chegada
		if(input_aux->dados[1] == n){ //fprintf(stderr, "Instante %d chega processo %d\n", n, input_aux->dados[0]);		
			if(aux2==NULL)
				aux2 = criar_tarefas(aux2, 1);
			*aux2 = *input_aux;
			aux2->next = NULL;
			if(ant!=NULL)
				ant->next = aux2;
			ant = aux2;
			aux2 = aux2->next;
		}
		
		input_aux = input_aux->next;

	}

}

/* ########################################################## */

int tamanho_alloc(){
	char c;
	int i = 1;

	while ((c = fgetc(input)) != EOF)
		if( c == '\n'){
			i++;
		}

	return i;
}

/* ########################################################## */

Tarefa *criar_tarefas(Tarefa *t, int n){
	int i;
	Tarefa *aux, *ant;

	aux = alocarTarefa();

	t = aux;
	ant = aux;
	aux = aux->next;


	for(i=1; i < n; i++){
		aux = alocarTarefa();
		ant->next = aux;
		ant = aux;
		aux = aux->next;
	}

	return t;
}

/* ########################################################## */

Tarefa *alocarTarefa(){
	Tarefa *temp;
	int i;

	temp = (Tarefa*) malloc (sizeof(Tarefa));
	temp->next = NULL;

	for(i=0; i < 5; i ++)
		temp->dados[i] = 0;

	return temp;
}

/* ########################################################## */

void fprinTest(Tarefa *t){
	Tarefa *aux = t;
	
	
	while(aux!=NULL){
		fprintf(stderr, "{%d;%d;%d;%d,%d}  ", aux->dados[0],aux->dados[1],aux->dados[2], aux->dados[3], aux->dados[4]);
		aux = aux->next;
	}
	fprintf(stderr, "\n");
}

/* ########################################################## */

float tempo_medio(Tarefa *t){
	int i;
	float tempo = 0;
	
	for(i=0; i < n_processos; i++)
		tempo += t[i].dados[2];
		
	return (tempo / n_processos);
	
}

/* ########################################################## */

void menor_tempo(){
	Tarefa *aux = lista_tarefas, *temp, *temp_ant, *ant=NULL;
	
	temp = aux;
	while(aux!=NULL){
		if(aux->dados[2] < temp->dados[2]){
			temp = aux;
			temp_ant = ant;
		}
		ant = aux;
		aux = aux->next;
	}
	
	if(temp != lista_tarefas){ //nao e o primeiro elemento
		temp_ant->next = temp->next;
		temp->next = lista_tarefas;
		lista_tarefas = temp;
	}
	

}
