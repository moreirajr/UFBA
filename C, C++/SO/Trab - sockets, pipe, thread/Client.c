#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <netdb.h>

#include <signal.h>
#include <pthread.h>



#define MAX_MSG 250

int socketfd;

/**************************************************************************************/

void sigint_handler(int sig){	//fecha socket ao finalizar o cliente
   close(socketfd);
   exit(sig);
}

/**************************************************************************************/

void *thread_rcv(){
	char buffer_temp[MAX_MSG];
	int size;
		
		while(1){
			memset(buffer_temp, 0, MAX_MSG);
			size = recv(socketfd, buffer_temp, MAX_MSG, 0);
			if( (size > 0) && (strlen(buffer_temp)>0) ){
				buffer_temp[size] = '\0';
				fprintf(stderr, "%s\n", buffer_temp);
			}
		}

		return NULL;
}

/**************************************************************************************/


char *comando(char *strp, int indice){
	int i=indice, len = strlen(strp);
	char *temp = (char*) malloc(sizeof(char) * len);

	while( (strp[i]!= ' ') && (i < len) && (strp[i]!='\n')){
		temp[i-indice] = strp[i];
		i++;
	}
	temp[i-indice] = '\0';

	return temp;
}

/**************************************************************************************/


int main (int argc, char *argv[]){	//3 argumentos, nome do cliente, endereco do sv e porta
	struct sockaddr_in servidor_end;
	struct hostent *servidor;
	int porta;
	char buffer[MAX_MSG];
	pthread_t thread1;


	if(argc <= 3){
		fprintf(stderr, "ERRO: numero de argumentos invalido\n");
		exit(1);
	}

	signal(SIGINT, sigint_handler);

	porta = atoi(argv[3]);
	bzero((char*) &servidor_end, sizeof(servidor_end));

	if( (socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("ERRO: socket.");
		exit(1);
	}

	servidor = gethostbyname(argv[2]);
	if(servidor== NULL){
		perror("ERRO: gethostbyname.");
	}

	servidor_end.sin_family = AF_INET;
	bcopy((char *)servidor->h_addr,(char *)&servidor_end.sin_addr.s_addr, servidor->h_length);
	servidor_end.sin_port = htons(porta);

	if( connect(socketfd, (struct sockaddr *) &servidor_end, sizeof(servidor_end)) == -1)
		perror("ERRO connect");
	else
	{
		fprintf(stderr, "Conectado com sucesso\n");
	
		/* enviar nome do cliente */
		if(send(socketfd, argv[1], MAX_MSG, 0) < 0)	//sizeof(argv[1])
			perror("ERRO: write.");

		srand(time(NULL));
		pthread_create(&thread1, NULL, thread_rcv, NULL);

	}

	while(1){
		fflush(stdin);
		memset(buffer, 0, MAX_MSG);
		fgets(buffer, MAX_MSG, stdin);

		if(send(socketfd, buffer, MAX_MSG, 0) < 0)
			perror("ERRO: write.");
			
	}//while

	close(socketfd);

	return 0;
}


