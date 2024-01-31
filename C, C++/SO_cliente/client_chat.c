#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet.h/in.h>

#define MAX 200

int main (int argc, char *argv[]){
	struct sockaddr_in servidor;
	struct hostent *server;	//server name
	int porta, socketFD, tam_string,
	size = sizeof(servidor);

	char buffer[MAX], *endereco = (char*) malloc(strlen(argv[2]) * sizeof(char) );

	//3 argumentos
	//nome do cliente, endereco do servidor, porta
	//comecando em arg[1]


	if ( (socketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("ERRO: socket.");
		exit(1);
	}

	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(porta);
	servidor.sin_addr.s_addr = inet_addr(endereco); //localhost

	memset(servidor.sin_zero, 0, 8);

	if( (connect(socketFD, (struct sockaddr*)&servidor, size)) == -1)
		perror("ERRO: connect.");
	else
		printf("Conectado com sucesso");


	strcpy(buffer, "Testando\n");


	while(1){
		if( (tam_string = recv(socketFD, buffer, MAX)) > 0){
			buffer[tam_string] = '\0';
			printf("\nMsg recebida : %s", buffer);
		}

		memset(buffer, 0, MAX);
		fgets(buffer, MAX, stdin);
		send(socketFD, buffer, strlen(buffer), 0);
	}

	close(socketFD);

	return 0;

}
