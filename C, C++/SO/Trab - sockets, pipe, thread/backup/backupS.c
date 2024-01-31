#include <stdio.h>
#include <stdlib.h>
#include <string.h>   //strlen

#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#include <time.h>
#include <signal.h>
#include <pthread.h>


#define MAX_CLIENTES 20
#define MAX_MSG 1000


typedef struct cli_{
	char nome[30];
	int soc;
} Clientes;

struct sockaddr_in servidor_end, cliente_end;
int socketfd, novo_socket, tam_endereco, fdn, p[2]; //pipe
fd_set readfd;
Clientes clientes[MAX_CLIENTES];
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;


/**************************************************************************************/

void sigint_handler(int sig){	//fecha socket ao finalizar o cliente
	int i;

	close(socketfd);
	for(i=0; i < MAX_CLIENTES; i++)
		close(i);
	exit(sig);
}

/**************************************************************************************/

char *mensagem(char *remetente, char *msg, int n){
	int i=n, len = strlen(msg);
	char temp2[MAX_MSG], temp[MAX_MSG];

	memset(temp, 0, MAX_MSG);
	memset(temp2,0, 100);

	strcpy(temp2, remetente);
	strcat(temp2, ": ");


	while( (i<len) && (msg[i]!='\n')){
		temp[i-n] = msg[i];
		i++;
	}
	temp[i-n] = '\0';

	return strcat(temp2, temp);
}

/**************************************************************************************/

struct tm *getHorario(){
	struct tm *temp_h;
	time_t hora;
	srand(time(NULL));

	hora = time(NULL);
	temp_h = localtime(&hora);

	return temp_h;
}

/**************************************************************************************/

char *comando(char *strp, int indice){
	int i=indice, len = strlen(strp);
	char *temp = (char*) malloc(sizeof(char) * len);

	memset(temp, 0, len);


	while( (strp[i]!= ' ') && (i < len) && (strp[i]!='\n')){
		temp[i-indice] = strp[i];
		i++;
	}
	temp[i-indice] = '\0';

	fprintf(stderr, "[COMANDO (%s) ]", temp);
	return temp;
}

/**************************************************************************************/

void *thread_accept(){ //T1
	int i, conexao_aceita=0;
	struct tm *temp_hora;
	char nome[30], buffer[MAX_MSG];

	//close, read e printf
	
	
	while(1){
		
		close(p[1]);
		read(p[0], &conexao_aceita, sizeof(conexao_aceita));
		fprintf(stderr,"aceitou: [%d]\n",conexao_aceita); 

	
		if ((novo_socket = accept(socketfd, (struct sockaddr *)&cliente_end, (socklen_t*)&tam_endereco))<0){
			perror("ERRO: accept.");
			exit(1);
		}
			

		
		memset(nome, 0, 30);
		recv(novo_socket, nome, 30, 0);
		temp_hora = getHorario();
		fprintf(stderr, "%.2d:%.2d \t %s \t Conectado\n", temp_hora->tm_hour, temp_hora->tm_min, nome);


		for (i = 0; i < MAX_CLIENTES; i++){
			if( clientes[i].soc == 0 ){				//posicao vazia
				//mutex
				clientes[i].soc = novo_socket;		//adiciona a lista de sockets
				strcpy(clientes[i].nome, nome);				
				break;
			}
		}
		
		
	}

	return NULL;
}

/**************************************************************************************/

void *thread_select(){ //T2
	int i, j, b_size, s_temp, atividade_socket, conexao_aceita=0;
	char buffer[MAX_MSG], nome[30], temp[7];
	struct tm *temp_hora;
					
	
	
	//ao aceitar uma nova conexao, acessar clientes (global) protegida por mutex
	

	while(1){
		FD_ZERO(&readfd);
		FD_SET(socketfd, &readfd);
		fdn = socketfd;
		
	
		for(i=0; i<MAX_CLIENTES; i++){
			s_temp = clientes[i].soc;	//global e protegida por mutex?? --mudar

			if(s_temp > 0)
				FD_SET(s_temp, &readfd);
				if(s_temp > fdn)		//necessario pro select, pegar o maior numero do file descriptor
					fdn = s_temp;
		}

		atividade_socket = select(fdn+1, &readfd, NULL, NULL, NULL);
		conexao_aceita = 0;

		if(atividade_socket <= 0)
			perror("ERRO: select.");

		if(FD_ISSET(socketfd, &readfd)){	//avisar ao outro thread
			close(p[0]);
			conexao_aceita=1;
			write(p[1], &conexao_aceita, sizeof(conexao_aceita));

		}
		
		//mutex

		//comandos
		for(i=0; i<MAX_CLIENTES; i++){
			s_temp = clientes[i].soc;
			if(FD_ISSET(s_temp, &readfd)){
				memset(buffer, 0, MAX_MSG);
				b_size = recv(s_temp, buffer, MAX_MSG, 0);
				fprintf(stderr, "MSG RECEBIDA -");
				
				if(b_size>0){ //msg recebida
					buffer[b_size] = '\0';
					fprintf(stderr, "cmd recebido(%s)\n", buffer);

					strcpy(temp, comando(buffer,0));
					if(strcmp(temp,"SEND")==0){
						strcpy(buffer, mensagem(clientes[i].nome, buffer, 5));
						for(j=0; j < MAX_CLIENTES; j++)	//enviar para todos clientes
							if( (clientes[j].soc != clientes[i].soc) && (clientes[j].soc!=0) ){
								fprintf(stderr, "\nEnviar a msg(%s) pra %s", buffer, clientes[j].nome);
								if(send(clientes[j].soc, buffer, MAX_MSG, 0) < 0)
									perror("ERRO: write.");
							}

					temp_hora = getHorario();
					fprintf(stderr, "%.2d:%.2d \t %s \t SEND \t Executado: \n", temp_hora->tm_hour, temp_hora->tm_min, clientes[i].nome);
					}
					else if(strcmp(temp,"SENDTO")==0){
						strcpy(nome, comando(buffer,7));//remetente
						fprintf(stderr," ->>>>> %s", nome);
						strcpy( buffer, mensagem(clientes[i].nome, buffer, strlen(nome)+7) );
						for(j=0; j < MAX_CLIENTES; j++)
							if( strcmp(clientes[j].nome,nome)==0)
								if(send(clientes[j].soc, buffer, MAX_MSG, 0) < 0)
									perror("ERRO: write.");
					}else if(strcmp(temp,"WHO")==0){
						for(j=0; j < MAX_CLIENTES; j++)
							if(clientes[j].soc!=0)
								if(send(clientes[i].soc, strcat(clientes[j].nome,"\n"), strlen(clientes[j].nome)+1, 0) < 0)
									perror("ERRO: write.");
					}
				}
				else{
					fprintf(stderr, "ENTRANDO NO ELSE\n");
					/*
					temp_hora = getHorario();
					fprintf(stderr, "%.2d:%.2d \t %s \t Conectado\n", temp_hora->tm_hour, temp_hora->tm_min, nome);
					*/
					close(s_temp);
					clientes[i].soc = 0;
				}
			}
		}//for
	}//while

	return NULL;
}

/**************************************************************************************/



/**************************************************************************************/

int main(int argc, char *argv[]){	//recebe um argumento - porta
	int i,porta, b_size, s_temp, atividade_socket, conexao_aceita=0, j;
	char buffer[MAX_MSG], temp_cmd[7],nome[30],  temp[7];
	pthread_t thread1, thread2;
	struct tm *temp_hora;

	
	//if( (strcmp(temp,"SEND")==0) || (strcmp(temp,"SENDTO")==0) || (strcmp(temp,"WHO")==0) || (strcmp(temp,"HELP")==0) ){}
	if(argc <= 1){
		fprintf(stderr, "ERRO: numero de argumentos invalido\n");
		exit(1);
	}


	signal(SIGINT, sigint_handler);
	pipe(p);
	
	
	for(i=0; i<MAX_CLIENTES; i++)
		clientes[i].soc = 0;


	porta = atoi(argv[1]);
	bzero((char *) &servidor_end, sizeof(servidor_end));
	FD_ZERO(&readfd);


	if( (socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("ERRO: socket.");
		exit(1);
	}

	servidor_end.sin_family = AF_INET;				//server byte order
	servidor_end.sin_addr.s_addr = INADDR_ANY;		//proprio endereco
	servidor_end.sin_port = htons(porta);			//porta

	if (bind(socketfd, (struct sockaddr *) &servidor_end, sizeof(servidor_end)) < 0){
		perror("ERRO:bind.");
		exit(1);
	}

	//receber conexoes, especifica o numero maximo de conexoes no socket master
	listen(socketfd, MAX_CLIENTES);

	tam_endereco = sizeof(cliente_end);
	

	//pthread_create(&thread1, NULL, thread_accept, NULL);
	//pthread_create(&thread2, NULL, thread_select, NULL);
	//pthread_join(thread1, NULL);
	//pthread_join(thread2, NULL);

/*
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */
	while(1){
		FD_ZERO(&readfd);
		FD_SET(socketfd, &readfd);
		fdn = socketfd;
		
	
		for(i=0; i<MAX_CLIENTES; i++){
			s_temp = clientes[i].soc;	//global e protegida por mutex?? --mudar

			if(s_temp > 0)
				FD_SET(s_temp, &readfd);
				if(s_temp > fdn)		//necessario pro select, pegar o maior numero do file descriptor
					fdn = s_temp;
		}

		atividade_socket = select(fdn+1, &readfd, NULL, NULL, NULL);
		conexao_aceita = 0;

		if(atividade_socket <= 0)
			perror("ERRO: select.");

		if(FD_ISSET(socketfd, &readfd)){	//avisar ao outro thread
			/*close(p[0]);
			conexao_aceita=1;
			write(p[1], &conexao_aceita, sizeof(conexao_aceita));
			*/
			
			
		if ((novo_socket = accept(socketfd, (struct sockaddr *)&cliente_end, (socklen_t*)&tam_endereco))<0){
			perror("ERRO: accept.");
			exit(1);
		}
			

		
		memset(nome, 0, 30);
		recv(novo_socket, nome, 30, 0);
		temp_hora = getHorario();
		fprintf(stderr, "%.2d:%.2d \t %s \t Conectado\n", temp_hora->tm_hour, temp_hora->tm_min, nome);


		for (i = 0; i < MAX_CLIENTES; i++){
			if( clientes[i].soc == 0 ){				//posicao vazia
				//mutex
				clientes[i].soc = novo_socket;		//adiciona a lista de sockets
				strcpy(clientes[i].nome, nome);				
				break;
			}
		}

		}
		
		//mutex

		//comandos
		for(i=0; i<MAX_CLIENTES; i++){
			s_temp = clientes[i].soc;
			if(FD_ISSET(s_temp, &readfd)){
				memset(buffer, 0, MAX_MSG);
				b_size = recv(s_temp, buffer, MAX_MSG, 0);
				fprintf(stderr, "MSG RECEBIDA -");
				
				if(b_size>0){ //msg recebida
					buffer[b_size] = '\0';
					fprintf(stderr, "cmd recebido(%s)\n", buffer);

					strcpy(temp, comando(buffer,0));
					if(strcmp(temp,"SEND")==0){
						strcpy(buffer, mensagem(clientes[i].nome, buffer, 5));
						for(j=0; j < MAX_CLIENTES; j++)	//enviar para todos clientes
							if( (clientes[j].soc != clientes[i].soc) && (clientes[j].soc!=0) ){
								fprintf(stderr, "\nEnviar a msg(%s) pra %s", buffer,clientes[j].nome);
								if(send(clientes[j].soc, buffer, MAX_MSG, 0) < 0)
									perror("ERRO: write.");
							}

					temp_hora = getHorario();
					fprintf(stderr, "%.2d:%.2d \t %s \t SEND \t Executado: \n", temp_hora->tm_hour, temp_hora->tm_min, clientes[i].nome);
					}
					else if(strcmp(temp,"SENDTO")==0){
						strcpy(nome, comando(buffer,7));//remetente
						fprintf(stderr," ->>>>> %s", nome);
						strcpy( buffer, mensagem(clientes[i].nome, buffer, strlen(nome)+7) );
						for(j=0; j < MAX_CLIENTES; j++)
							if( strcmp(clientes[j].nome,nome)==0)
								if(send(clientes[j].soc, buffer, MAX_MSG, 0) < 0)
									perror("ERRO: write.");
					}else if(strcmp(temp,"WHO")==0){
						for(j=0; j < MAX_CLIENTES; j++)
							if(clientes[j].soc!=0)
								if(send(clientes[i].soc, strcat(clientes[j].nome,"\n"), strlen(clientes[j].nome)+1, 0) < 0)
									perror("ERRO: write.");
					}
				}
				else{
					fprintf(stderr, "ENTRANDO NO ELSE\n");
					/*
					temp_hora = getHorario();
					fprintf(stderr, "%.2d:%.2d \t %s \t Conectado\n", temp_hora->tm_hour, temp_hora->tm_min, nome);
					*/
					close(s_temp);
					clientes[i].soc = 0;
				}
			}
		}//for
	}//while

	close(socketfd);
	for(i=0; i < MAX_CLIENTES; i++)
		close(i);

	return 0;
}
