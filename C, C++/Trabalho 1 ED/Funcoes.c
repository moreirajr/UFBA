/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tipos.h"


void bzero_macro(char *string, int lenght){
	while(lenght-- > 0)
		*string++ = 0;	// 0 = null (ASCII)
}

/***********************************************************************************************/

char *comando(char *comando, TIPO t){
	char nome_temp[21], idade_temp[3];
	int i=2, n;


	bzero_macro(nome_temp, 21);
	bzero_macro(idade_temp, 3);


	if(t==consulta){
		while(comando[i]!='\0'){

			i++;
		}
		i--;
	}
	else
		while(comando[i]!=32){ //i=2, primeiro caractere do nome
			i++;				// para adicionar amigos, pega o primeiro nome
		}

	n=i;	//indice do vetor onde o segundo espaco (caractere space) e encontrado

	if(t!=nome2)
	for(i=2; i<n; i++ )
		nome_temp[i-2] = comando[i];

	if(t==nome)
		nome_temp[n]='\0';	//nao copiar o caractere espaco, cadastro


	if(t==nome || t==consulta)
		return nome_temp;

	if(t==idade){
		for (i=n+1; i<(strlen(comando)-1); i++) // n+1 = primeiro caractere encontrado depois do nome
			if( isdigit(comando[i]) )
				idade_temp[i-n-1] = comando[i];

		return idade_temp;
	}

	if(t==nome2){	//segundo nome
		for(i=n+1; i<(strlen(comando)-1); i++)
			nome_temp[i-n-1] = comando[i];
	}

	return nome_temp;

}


/***********************************************************************************************/

User_data *cadastrar_usuario(User_data *u, char *nome_temp, int idade){

	User_data *temp, *temp_aux, *temp_ant;

	temp = (User_data*)malloc(sizeof(User_data));

	bzero_macro(temp->nome, 21);


	strcpy(temp->nome, nome_temp);
	temp->idade = idade;
	temp->qnt_grupos = 0;
	temp->amigos_us = NULL;
	temp->grupos_us = NULL;
	temp->prox = NULL;

	if(u==NULL){
		temp->ant = NULL;
		u = temp;
	}

	else{

		temp_aux = u;
		while(temp_aux!=NULL){
			temp_ant = temp_aux;
			temp_aux = temp_aux->prox;
		}

		temp->ant = temp_ant;
		temp_ant->prox = temp;

	}
	return u;
}

/***********************************************************************************************/

User_data *consulta_usuario(User_data *u, char *nome){
	User_data *temp = u;

	if(temp==NULL)
		return NULL;	//

	while(temp!=NULL){
		if( (strcmp(temp->nome, nome)==0) ){ //usuario encontrado
			return (temp);
		}
		temp=temp->prox;
	}

	return NULL;
}

/***********************************************************************************************/

User_data *remove_usuario(User_data *u, User_data *remover){
	User_data *temp, *temp_ant, *temp_prox, *usuario_temp;
	Amigos *amigos_temp = remover->amigos_us;


		if(amigos_temp!=NULL) //o usuario possui amigos adicionados
		while(amigos_temp!=NULL){
			usuario_temp = consulta_usuario(u, amigos_temp->nome);
			remove_amigo(u, remover, usuario_temp); //remover amigos
			amigos_temp = amigos_temp->prox;
		}


	if(strcmp(u->nome, remover->nome)==0){
		if(u->prox==NULL){ //apenas um usuario cadastrado
			free(u);
			return u;
		}
		temp = u;  //o usuario a ser removido e´o primeiro
		u = u->prox;
		u->ant = NULL;
		free(temp);
	}
	else{

		temp = u;
		while( (strcmp(temp->nome, remover->nome)!=0 ) && temp!=NULL){
			temp = temp->prox;
			temp_prox = temp->prox;	//proximo no, depois de temp
			temp_ant = temp->ant;	//no anterior a temp
		}

		temp_prox->ant = temp_ant;
		temp_ant->prox = temp_prox;
		free(temp);	//remove temp
	}

	return u;
}

/***********************************************************************************************/

User_data *adicionar_amigo(User_data *u, User_data *u2, User_data *usuarios){
	Amigos *amigos_ant, *amigos_aux, *amigos_ant2, *amigos_aux2,
	*amigos_temp = (Amigos*)malloc(sizeof(Amigos)),
	*amigos_temp2 = (Amigos*)malloc(sizeof(Amigos));

	strcpy(amigos_temp->nome, u2->nome);
	strcpy(amigos_temp2->nome, u->nome);

	/*amigos adicionados em ordem alfabetica, primeiro usuario*/

	if(u->amigos_us==NULL){
		amigos_temp->prox = NULL;
		u->amigos_us = amigos_temp;
	}

	else{
		if(strcmp(amigos_temp->nome, u->amigos_us->nome)<0){ // amigos_temp->nome aparece primeiro
			amigos_temp->prox = u->amigos_us;            //na ordem alfabetica
			u->amigos_us = amigos_temp;
		}

		else{

			amigos_aux = u->amigos_us;
			amigos_ant = u->amigos_us;
			while(amigos_aux!=NULL){
				if(strcmp(amigos_temp->nome, amigos_aux->nome)<0) //adicionar em ordem alfabetica
					break;
				amigos_ant = amigos_aux;
				amigos_aux = amigos_aux->prox;
			}

			amigos_temp->prox = amigos_aux;//o proximo
			amigos_ant->prox = amigos_temp;
		}
	}

	/******************* segundo usuario**********************/

	if(u2->amigos_us==NULL){
		amigos_temp2->prox = NULL;
		u2->amigos_us = amigos_temp2;
	}

	else{
		if(strcmp(amigos_temp2->nome, u2->amigos_us->nome)<0){
			amigos_temp2->prox = u2;
			u2->amigos_us = amigos_temp2;
		}
		else{
			amigos_aux2 = u2->amigos_us;
			amigos_ant2 = u2->amigos_us;

			while(amigos_aux2!=NULL){
				if(strcmp(amigos_temp2->nome, amigos_aux2->nome)<0)
					break;
				amigos_ant2 = amigos_aux2;
				amigos_aux2 = amigos_aux2->prox;
			}

			amigos_temp2->prox = amigos_aux2;//o proximo
			amigos_ant2->prox = amigos_temp2;
		}
	}
	return usuarios;
}

/***********************************************************************************************/

void listar_amigos(User_data *u, char *nome){
	Amigos *amigo_temp;


	printf("amigos de: %s", nome);

	if(u->amigos_us==NULL)
		return;
	else {

		amigo_temp = u->amigos_us;
		while(amigo_temp!=NULL){
			printf("\n%s", amigo_temp->nome);
			amigo_temp = amigo_temp->prox;
		}
	}
}

/***********************************************************************************************/

User_data *remove_amigo(User_data *u, User_data *u1, User_data *u2){
	Amigos *amigos_ant, *amigos_aux, *amigos_prox, *amigos_ant2, *amigos_aux2, *amigos_prox2;


	if( (u1->amigos_us==NULL) || (u2->amigos_us==NULL)) //um dos usuarios nao possui amigos
		return u;


	if(strcmp(u1->amigos_us->nome, u2->nome)==0){
		amigos_aux = u1->amigos_us;
		u1->amigos_us = u1->amigos_us->prox;
		free(amigos_aux);
	}
	else{
		amigos_aux = u1->amigos_us;
		//
		while((amigos_aux!=NULL) && (strcmp(u2->nome, amigos_aux->nome)!=0)){
			amigos_ant = amigos_aux;
			amigos_aux = amigos_aux->prox;
			amigos_prox = amigos_aux->prox;
		}

		amigos_ant->prox = amigos_prox;
		free(amigos_aux);
	}

	if(amigos_aux==NULL)	//os dois usuarios nao sao amigos
		return u;

	if(strcmp(u2->amigos_us->nome, u1->nome)==0){
		amigos_aux2 = u2->amigos_us;
		u2->amigos_us = u2->amigos_us->prox;
		free(amigos_aux2);
	}
	else{
		amigos_aux2 = u2->amigos_us;
		while((amigos_aux2!=NULL) && (strcmp(u1->nome, amigos_aux2->nome)!=0)){
			amigos_ant2 = amigos_aux2;
			amigos_aux2 = amigos_aux2->prox;
			amigos_prox2 = amigos_aux2->prox;
		}

		amigos_ant2->prox = amigos_prox2;
		free(amigos_aux2);
	}

	return u;
}

/***********************************************************************************************/

Grupos_Amigos *criar_grupos(Grupos_Amigos *g, char *nome){
	Grupos_Amigos *grupo_temp, *grupo_aux,
	*temp = (Grupos_Amigos*)malloc(sizeof(Grupos_Amigos));


	if(g==NULL){
		strcpy(temp->nome, nome);
		temp->prox=NULL;
		temp->membros=NULL;
		g=temp;
	}
	else{
		grupo_temp = g;
		while(grupo_temp!=NULL){
			grupo_aux = grupo_temp;
			grupo_temp = grupo_temp->prox;
		}

		strcpy(temp->nome, nome);
		temp->membros = NULL;
		temp->prox = NULL;
		grupo_aux->prox = temp;
	}
		return g;
}

/***********************************************************************************************/

Grupos_Amigos *adicionar_ao_grupo(Grupos_Amigos *g, User_data *usuario){
	Amigos *amigos_aux, *amigos_temp,
	*temp = (Amigos*)malloc(sizeof(Amigos));
	Grupos_Amigos *grupo_temp, *grupo_aux, *grupo_ant;

	bzero_macro(temp->nome, 21);

	if(usuario->qnt_grupos < 5){ //numero maximo de grupos que o usuario pode participar
		strcpy(temp->nome, usuario->nome);
		if(g->membros==NULL){
			temp->prox=NULL;
			g->membros = temp;
		}

	else{

		if(strcmp(temp->nome, g->membros->nome)<0){
			temp->prox = g->membros;
			g->membros = temp;
		}

		else{
			amigos_aux = g->membros;
			amigos_temp = g->membros;
			while(amigos_aux!=NULL){
				if(strcmp(temp->nome, amigos_aux->nome)<0)
					break;
				amigos_temp = amigos_aux;
				amigos_aux = amigos_aux->prox;
			}
			temp->prox= amigos_aux;
			amigos_temp->prox = temp;
		}

	}

		/*estrutura dos usuarios*/
	grupo_temp = (Grupos_Amigos*)malloc(sizeof(Grupos_Amigos));
	if(usuario->grupos_us==NULL){
		strcpy(grupo_temp->nome, g->nome);
		grupo_temp->prox = NULL;
		usuario->qnt_grupos++;
		usuario->grupos_us = grupo_temp;
	}
	else{
			grupo_aux = usuario->grupos_us;
			while(grupo_aux!=NULL){
				grupo_ant = grupo_aux;
				grupo_aux = grupo_aux->prox;
			}

			strcpy(grupo_temp->nome, g->nome);
			grupo_temp->prox = NULL;
			usuario->qnt_grupos++;
			grupo_ant->prox = grupo_temp;
		}
	}
	return g;
}

/***********************************************************************************************/

Grupos_Amigos *consultar_grupo(Grupos_Amigos *g, char *nome){
	Grupos_Amigos *temp;

	if(g==NULL)
		return (NULL);
	else{
		temp = g;
		while((temp!=NULL) && (strcmp(temp->nome, nome)!=0))
			temp = temp->prox;

	}
	return(temp);
}

/***********************************************************************************************/

int listar_membros(Grupos_Amigos *g, char *nome, TIPO t){
	Amigos *temp;

	if(t!=consulta)
		printf("grupo: %s", g->nome);

	if(g->membros==NULL)
		return 0;

	temp = g->membros;

	if(t!=consulta){
		while(temp!=NULL){
			printf("\n%s", temp->nome);
			temp = temp->prox;
		}
	}
	else{
		while(temp!=NULL){
			if(strcmp(nome, temp->nome)==0)
				return 1;
			temp = temp->prox;
		}
	}
	return 0;
}

/***********************************************************************************************/

Grupos_Amigos *remover_grupo(Grupos_Amigos *g, char*nome, User_data *usuarios){
	Grupos_Amigos *temp, *temp_prox, *temp_ant;
	User_data *user_temp;


	if(g->membros!=NULL){ //remover usuarios do grupo
		while(g->membros!=NULL){
			user_temp = consulta_usuario(usuarios, g->membros->nome);
			remover_do_grupo(g, user_temp);
			g->membros = g->membros->prox;//
		}
	}

	if(strcmp(g->nome, nome)==0){
		if(g->prox==NULL){
			free(g);
			return g;
		}
	temp = g;
	g = g->prox;
	free(temp);
	}

	else{
		temp = g;
		while( (strcmp(temp->nome, nome)!=0 ) && temp!=NULL){
			temp_ant = temp;
			temp = temp->prox;
			temp_prox = temp->prox;
		}

		temp_ant->prox = temp_prox;
		free(temp);
	}
	return g;
}




/***********************************************************************************************/

Grupos_Amigos *remover_do_grupo(Grupos_Amigos *g, User_data *usuario){
	Amigos *temp, *temp_ant, *temp_prox;
	Grupos_Amigos *grupo_temp, *grupo_ant, *grupo_prox, *grupo_aux;

	if(usuario->grupos_us==NULL) //o usuario nao pertence ao grupo
		return NULL;

	if(strcmp(usuario->grupos_us->nome, g->nome)==0){	//grupos do usuario
		if(usuario->grupos_us->prox==NULL){
			bzero_macro(usuario->grupos_us->nome,21);
			usuario->grupos_us=NULL;
		}
		else{
//
			grupo_aux = usuario->grupos_us;
			usuario->grupos_us = usuario->grupos_us->prox;
			free(grupo_aux);

		}
	}

	else{
		grupo_temp = usuario->grupos_us;
		while((grupo_temp!=NULL) && (strcmp(grupo_temp->nome, g->nome)!=0)){
			grupo_ant = grupo_temp;
			grupo_temp = grupo_temp->prox;
			grupo_prox = grupo_temp;
		}
		grupo_ant->prox = grupo_prox;
		free(grupo_temp);

	}

	(usuario->qnt_grupos)--; //numero de grupos que o usuario participa


	if(strcmp(g->membros->nome, usuario->nome)==0){
		if(g->membros->prox==NULL){
			bzero_macro(g->membros->nome, 21);
			g->membros=NULL;	//erro ao usar free(g->membros)

		}
		else{
			temp = g->membros;
			g->membros = g->membros->prox;
			free(temp);
		}
	}
	else{

		temp = g->membros;
		while( (strcmp(temp->nome, usuario->nome)!=0 ) && temp!=NULL){
			temp_ant = temp;
			temp = temp->prox;
			temp_prox = temp->prox;
		}

		temp_ant->prox = temp_prox;
		free(temp);
	}

	return g;
}

/***********************************************************************************************/

Grupos_Amigos *unir_grupos(Grupos_Amigos *g, Grupos_Amigos *g2, Grupos_Amigos *grupos, User_data *u){
	Amigos *temp = g2->membros;
	User_data *usuario_temp;

	if(temp==NULL){ //o grupo 2 nao possui membros
		goto delete_;
	}


	while(temp!=NULL){
		if(listar_membros(g, temp->nome, consulta)==0){	//membro nao adicionado no grupo 1
			usuario_temp = consulta_usuario(u, temp->nome);
			adicionar_ao_grupo(g, usuario_temp);
		}
		temp = temp->prox;
	}
							/* Deletar o grupo 2*/
	delete_:	remover_grupo(grupos, g2->nome, u);
	return grupos;
}


/***********************************************************************************************/

Grupos_Amigos *comparar_grupos(Grupos_Amigos *g, Grupos_Amigos *g2){
	Grupos_Amigos *temp=g, *temp2;

	if((g==NULL)|| (g2==NULL))
		return NULL;

	while(temp!=NULL){
		temp2 = g2;
		while(temp2!=NULL){
			if(strcmp(temp->nome, temp2->nome)==0)
				return temp;
			temp2 = temp2->prox;
		}
		temp = temp->prox;
	}
	return NULL;
}
/***********************************************************************************************/
