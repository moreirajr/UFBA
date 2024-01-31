//testando: nc localhost numerodaporta

//http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#socket

#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <time.h>


#define MAX_MSG 200
#define MAX_CLIENTES 100

int main (int argc, char *argv[]){
	struct sockaddr_in endereco;
	fd_set readfds;//set of socket descriptors
	int porta, i, s_socket, cliente_sockets[MAX_CLIENTES], opt=1, tam_endereco,
		s_temp, fdn, atv_socket, novo_socket, valor_read;
	time_t hora;
	struct tm temp_h;

	char buffer[MAX_MSG];

	porta = atoi(argv[1]);

	for(i=0; i < MAX_CLIENTES; i++)	//inicializacao dos sockets
		cliente_sockets[i] = 0;

	//socket(dominio, tipo, protocolo)
	if( (s_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0 ){
		perror("ERRO: socket.");
		exit(1);
	}


    if (setsockopt(s_socket, SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int)) == -1) {
        perror("ERRO: setsockopt.");
        exit(1);
    }

    //definir o tipo do socket:
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INNADDR_ANY;
    endereco.sin_port = htons(porta);

    //bind socket a porta
    if (bind(s_socket, (struct sockaddr *)&endereco, sizeof(endereco))<0){
            perror("ERRO: bind.");
            exit(1);
    }

    //listener na porta 'porta'

    //definindo o numero maximo de conexoes para o socket (max_clientes)
    if (listen(s_socket, MAX_CLIENTES) < 0){
    	perror("ERRO: listen.");
    	exit(1);
    }

    tam_endereco = sizeof(endereco);
    //esperando conexao

    while(1){
    	FD_ZERO(&readfds);			//macro para limpar o socket set
    	FD_SET(s_socket, &readfds);	//adiciona s_socket ao set
    	fdn = s_socket;				//fdn = file descriptor number

    	for(i=0; i < MAX_CLIENTES; i++){
    		s_temp = cliente_sockets[i];	//socket descriptor

    		if(s_temp > 0)
    			FD_SET(s_temp, &readfds);	//add na read list

    		if(s_temp > fdn)	//pegar o maior numero do file descriptor (utilizar no select)
    			fdn = s_temp;
    	}

    	//esperar por uma atividade de um dos sockets, timeout e null
    	atv_socket = select(fdn + 1 , &readfds , NULL , NULL , NULL);

    	 if ((atv_socket < 0) && (errno!=EINTR)){
    		 printf("ERRO: select.");
    	 }

    	 //If something happened on the master socket , then its an incoming connection
    	 if (FD_ISSET(s_socket, &readfds)){
    		 if ((novo_socket = accept(s_socket, (struct sockaddr *)&endereco, (socklen_t*)&tam_endereco))<0){
    			 perror("ERRO: accept.");
    			 exit(1);
    		 }

    		 //inform user of socket number - used in send and receive commands
   // printf("New connection , socket fd is %d , ip is : %s , port : %d \n" ,novo_socket , inet_ntoa(endereco.sin_addr) , ntohs(endereco.sin_port));

    		 hora = time(NULL);
    		 temp_h = localtime(&hora);
    		 printf("%.2d:.2%d \t CLIENTE_NAME \t Conectado", temp_h.tm_hour, temp_h.tm_min);
    		 //send new connection greeting message
    		 if( send(novo_socket, message, strlen(message), 0) != strlen(message) )
    			 perror("ERRO: send.");

             //add new socket to array of sockets
    		 for (i = 0; i < MAX_CLIENTES; i++){
    			 if( cliente_sockets[i] == 0 ){				//if position is empty
    	        	 cliente_sockets[i] = novo_socket;		//adiciona a lista de sockets
                     break;
                 }
             }
    	 }//FD_ISSET

    	 //else its some IO operation on some other socket :)
    	 for(i = 0; i < MAX_CLIENTES; i++){
    		 s_temp = cliente_sockets[i];
    		 if( FD_ISSET(s_temp , &readfds) ){
    			 //Check if it was for closing , and also read the incoming message
    	         if( (valor_read = read(s_temp , buffer, MAX_MSG)) == 0){
    	        	 //Somebody disconnected , get his details and print
    	             getpeername(s_temp , (struct sockaddr*)&endereco , (socklen_t*)&tam_endereco);
    	             printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(endereco.sin_addr) , ntohs(endereco.sin_port));
                     //Close the socket and mark as 0 in list for reuse
                     close(s_temp);
                      cliente_sockets[i] = 0;
    	         }

              //Echo back the message that came in
                else{
    	             //set the string terminating NULL byte on the end of the data read
    	             buffer[valor_read] = '\0';
    	             send(s_temp , buffer , strlen(buffer) , 0 );
                }
    		 }
    	 }
}//while


	return 0;
}


/*
 Suporta os seguintes comandos (a partir do clinete)

 Send(message) : envia <cliente_name> : <message> para todos os clientes conectados, menos o emissor

 SendTo(client_name, message): igual ao send, porem envia a msg somente pro cliente especificado

 who(): retorna a lista dos clientes conectados ao servidor

 help(): retorna a lista de comandos suportados e seu uso


 pegar nome e colocar nome no socket:

 int getsockopt(int s, int level, int optname, void *optval,
               socklen_t *optlen);
int setsockopt(int s, int level, int optname, const void *optval,
               socklen_t optlen);

 */

//o servidor deve usar dois threads, T1 e T2

//T1 espera as conexoes com accept()
//T2 gerencia as conexoes abertas usando select()
//um pipe() p entre T1 e T2 informa a T2 quando T1 aceitou uma nova conexao. (e T2 acessa uma variavel protegida por mutex)



//int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *utimeout);



//http://www.binarytides.com/multiple-socket-connections-fdset-select-linux/



