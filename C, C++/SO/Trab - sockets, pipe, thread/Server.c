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
#define MAX_MSG 250


typedef struct cli_{
	char nome[30];
	int soc;
} Clientes;

struct sockaddr_in servidor_end, cliente_end;
int masterSocket, novo_socket, tam_endereco, max_socketfd, fd[2];
fd_set readfd;
Clientes clientes[MAX_CLIENTES];
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

const char* CMD_HELP = "Comandos suportados pelo servidor: SEND, SENDTO, WHO, HELP";

/**************************************************************************************/

void sigint_handler(int sig){	//fecha socket ao finalizar o cliente
	int i;

	close(masterSocket);
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

	/* n = onde comeca a msg (recebe [CMD MSG] => [SEND hello] */
	while( (i<len) && (msg[i]!='\n')){
		temp[i-n] = msg[i];
		i++;
	}
	temp[i-n] = '\0';

	return strcat(temp2, temp);
}

/**************************************************************************************/

struct tm *HorarioAtual(){
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
	
	return temp;
}

/**************************************************************************************/

void *thread_accept(){ //T1
	int i, j, b_size, conexao_aceita=0, s_temp;
	struct tm *temp_hora;
	char nome[30], buffer[MAX_MSG];


	if ((novo_socket = accept(masterSocket, (struct sockaddr *)&cliente_end, (socklen_t*)&tam_endereco))<0){
			perror("ERRO: accept.");
			exit(1);
	}
		
	memset(nome, 0, 30);
	recv(novo_socket, nome, MAX_MSG, 0);
	temp_hora = HorarioAtual();
	fprintf(stderr, "%.2d:%.2d \t %s \t Conectado\n", temp_hora->tm_hour, temp_hora->tm_min, nome);

	for (i = 0; i < MAX_CLIENTES; i++){
		if( clientes[i].soc == 0 ){				//posicao vazia
			//mutex?
			clientes[i].soc = novo_socket;		/* adiciona na lista de sockets */
			strcpy(clientes[i].nome, nome);				
			break;
		}
	}
		
	return NULL;
}

/**************************************************************************************/

void *thread_select(){ //T2
	int i, j, s_temp, atividade_socket, b_size, executado;
	char buffer[MAX_MSG], nome[30], temp[7];
	pthread_t thread2;
	struct tm *temp_hora;
				
	while(1){
		FD_ZERO(&readfd);
		FD_SET(masterSocket, &readfd);
		max_socketfd = masterSocket;		
	
		for(i=0; i<MAX_CLIENTES; i++){
			s_temp = clientes[i].soc;	//global e protegida por mutex?? --mudar

	/* necessario pro select, pegar o maior numero do file descriptor */
			if(s_temp > 0)
				FD_SET(s_temp, &readfd);
				if(s_temp > max_socketfd)		
					max_socketfd = s_temp;
		}

		atividade_socket = select(max_socketfd+1, &readfd, NULL, NULL, NULL);
		
		if(atividade_socket <= 0)
			perror("ERRO: select.");

		/* THREAD ACCEPT */
		if(FD_ISSET(masterSocket, &readfd)){
			pthread_create(&thread2, NULL, thread_accept, NULL);
			pthread_join(thread2, NULL);
		}

		for(i=0; i<MAX_CLIENTES; i++){
			s_temp = clientes[i].soc;
			if(FD_ISSET(s_temp, &readfd)){
				memset(buffer, 0, MAX_MSG);
				b_size = recv(s_temp, buffer, MAX_MSG, 0);
				executado = 0;
				
				/* msg recebida */				
				if(b_size>0){ 
					buffer[b_size-1] = '\0';					
					strcpy(temp, comando(buffer,0));
					
					if(strcmp(temp,"SEND")==0){
						strcpy(buffer, mensagem(clientes[i].nome, buffer, 5));
						for(j=0; j < MAX_CLIENTES; j++)	/* enviar para todos clientes */
							if( (clientes[j].soc != clientes[i].soc) && (clientes[j].soc!=0) ){
								//fprintf(stderr, "\nEnviar a msg(%s) pra %s", buffer,clientes[j].nome);
								if(send(clientes[j].soc, buffer, MAX_MSG, 0) < 0)
									perror("ERRO: write.");
							}

					temp_hora = HorarioAtual();
					fprintf(stderr, "%.2d:%.2d \t %s \t SEND \t Executado: \n", temp_hora->tm_hour, temp_hora->tm_min, clientes[i].nome);

					}
					else if(strcmp(temp,"SENDTO")==0){
						strcpy(nome, comando(buffer,7));//remetente
						strcpy( buffer, mensagem(clientes[i].nome, buffer, strlen(nome)+7) );
						for(j=0; j < MAX_CLIENTES; j++)
							if( strcmp(clientes[j].nome,nome)==0)
								if(send(clientes[j].soc, buffer, MAX_MSG, 0) < 0)
									perror("ERRO: write.");
					}else if(strcmp(temp,"WHO")==0){
						char temp_clientes[1000];
						memset(temp_clientes, 0, 1000);
						for(j=0; j < MAX_CLIENTES; j++)
							if(clientes[j].soc!=0){
								strcat(temp_clientes, clientes[j].nome);
								strcat(temp_clientes, "\n");
							}
							if(send(clientes[i].soc, temp_clientes, strlen(temp_clientes)+1, 0) < 0)
								perror("ERRO: write.");
					}
					else if(strcmp(temp,"HELP")==0){
						//if(send(clientes[i].soc, CMD_HELP, strlen(CMD_HELP)+1, 0) < 0)
							//perror("ERRO: write.");
					}
				}
				else{
					temp_hora = HorarioAtual();
					fprintf(stderr, "%.2d:%.2d \t %s \t Desconectado\n", temp_hora->tm_hour, temp_hora->tm_min, clientes[i].nome);
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
	int i,porta, b_size, s_temp, atividade_socket, j;
	char buffer[MAX_MSG], temp_cmd[7],nome[30],  temp[7];
	pthread_t thread1; //, thread2;
	struct tm *temp_hora;

	
	if(argc <= 1){
		fprintf(stderr, "ERRO: numero de argumentos invalido\n");
		exit(1);
	}

	signal(SIGINT, sigint_handler);
	
	if(pipe(fd)!=0){
		perror("ERRO: pipe.");
		exit(1);
	}	
	
	for(i=0; i<MAX_CLIENTES; i++)
		clientes[i].soc = 0;


	porta = atoi(argv[1]);
	bzero((char *) &servidor_end, sizeof(servidor_end));
	FD_ZERO(&readfd);


	if( (masterSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("ERRO: socket.");
		exit(1);
	}
	
	//resolver problema do Bind: Address already in use.
	int enable = 1;
	if (setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		error("ERRO: setsockopt(SO_REUSEADDR).");

	servidor_end.sin_family = AF_INET;				//server byte order
	servidor_end.sin_addr.s_addr = INADDR_ANY;		//proprio endereco
	servidor_end.sin_port = htons(porta);			//porta

	if (bind(masterSocket, (struct sockaddr *) &servidor_end, sizeof(servidor_end)) < 0){
		perror("ERRO:bind.");
		exit(1);
	}

	//receber conexoes, especifica o numero maximo de conexoes no socket master
	listen(masterSocket, MAX_CLIENTES);
 
	tam_endereco = sizeof(cliente_end);
	
	//ou &thread_select?
	pthread_create(&thread1, NULL, thread_select, NULL);
	pthread_join(thread1, NULL);
	
	//close(masterSocket);
	//for(i=0; i < MAX_CLIENTES; i++)
	//	close(i);

	return 0;
}
