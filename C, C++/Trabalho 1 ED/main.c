/* Compilador: MinGW (GCC)
 * Aluno: Evaldo Machado Moreira Junior
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "tipos.h"


int main(void){

	char buffer[48], nome_temp[21], idade_temp[3], nome_temp2[21];
	User_data *usuarios=NULL, *usuario_temp, *usuario_temp2;
	Grupos_Amigos *grupos=NULL, *grupos_temp, *grupos_temp2;
	int idade_;

/* nome_temp: primeira palavra apos a letra que simboliza o comando, nome_temp2: segunda palavra
 *
 */

	/*printf("\n\n\t\tInforme o comando desejado:\n\n");
	printf("p: Cadastrar usuario.\nc: Consultar nome de usuario.\nr: Remover usuario.\na: Adicionar amigo a um usuario.\n");
	printf("q: Consultar amigos de um usuario.\nd: Remover amigo de um usuario.\ng: Criar grupo de amigos.");
	printf("\ni: Inserir usuario em um grupo.\nl: Listar membros de grupo.\nh: Remover usuario do grupo.\ns: Remover grupo.\n");
	printf("u: Unir grupos.\nv: Comparar grupos.\ne: Sair.\n\n");
	*/

	while(buffer[0]!=101) { // caractere "e" (ASCII)
		bzero_macro(buffer, 48);
		bzero_macro(nome_temp, 21);
		bzero_macro(nome_temp2, 21);
		bzero_macro(idade_temp, 3);
		usuario_temp=NULL, usuario_temp2=NULL, grupos_temp=NULL, grupos_temp2=NULL;
		fgets(buffer, 48, stdin);

		if( (buffer[1]==32) && (strlen(buffer)>=3) && (buffer[2]!=32) ) // 32 = espaco (ASCII)
			switch(buffer[0]){

		case 'p':

			strcpy(nome_temp, (comando(buffer, nome)));
			strcpy(idade_temp, (comando(buffer, idade)));
			idade_ = atoi(idade_temp);	//converter a idade do tipo caractere para inteiro

			if(((usuario_temp=consulta_usuario(usuarios, nome_temp))==NULL) && (idade_>=0))
				usuarios = cadastrar_usuario(usuarios, nome_temp, idade_);
			else{
				if(usuario_temp!=NULL)
					printf("pessoa jah cadastrada: %s", nome_temp);
			}
			break;

		case 'c':

			strcpy(nome_temp, (comando(buffer, consulta)));
			if((usuario_temp=consulta_usuario(usuarios, nome_temp))==NULL)
				printf("pessoa nao cadastrada: %s", nome_temp);
			else
				printf("%s %d", nome_temp, usuario_temp->idade);
			break;

		case 'r':
			strcpy(nome_temp, (comando(buffer, consulta)));
			if ((usuario_temp=consulta_usuario(usuarios, nome_temp))!=NULL){
				grupos_temp2 = usuario_temp->grupos_us;
				while(grupos_temp2!=NULL){
					grupos_temp = consultar_grupo(grupos, usuario_temp->grupos_us->nome);
					remover_do_grupo(grupos_temp, usuario_temp);
					grupos_temp2 = grupos_temp2->prox;
				}
				remove_usuario(usuarios, usuario_temp);
			}
			else
				printf("pessoa nao cadastrada: %s", nome_temp);
			break;

		case 'a':

			strcpy(nome_temp, (comando(buffer, nome)));
			strcpy(nome_temp2,(comando(buffer, nome2)));
			if((strcmp(nome_temp,nome_temp2)!=0) && (usuarios!=NULL)){
				if((usuario_temp=consulta_usuario(usuarios, nome_temp))!=NULL){
					if((usuario_temp2=consulta_usuario(usuarios, nome_temp2))!=NULL)
						usuarios = adicionar_amigo(usuario_temp, usuario_temp2, usuarios);
					else
						printf("pessoa nao cadastrada: %s", nome_temp2);
				}
				else
					printf("pessoa nao cadastrada: %s", nome_temp);
			}
			break;

		case 'q':

			strcpy(nome_temp, comando(buffer, consulta));
			if((usuario_temp = consulta_usuario(usuarios, nome_temp))!=NULL)
				listar_amigos(usuario_temp, nome_temp);
			else
				printf("pessoa nao cadastrada: %s", nome_temp);
			break;

		case 'd':

			strcpy(nome_temp, (comando(buffer, nome)));
			strcpy(nome_temp2,(comando(buffer, nome2)));
			if(strcmp(nome_temp,nome_temp2)!=0){//nomes diferentes
				if((usuario_temp=consulta_usuario(usuarios, nome_temp))!=NULL){
					if((usuario_temp2=consulta_usuario(usuarios, nome_temp2))!=NULL)
						remove_amigo(usuarios, usuario_temp, usuario_temp2);
					else
						printf("pessoa nao cadastrada: %s", nome_temp2);
					}
				else
					printf("pessoa nao cadastrada: %s", nome_temp);
			}
				break;

		case 'g':

			strcpy(nome_temp, (comando(buffer, consulta)));
			if(consultar_grupo(grupos, nome_temp)==NULL)
				grupos = criar_grupos(grupos, nome_temp);
			else
				printf("grupo jah cadastrado: %s", nome_temp);
			break;

		case 'i':

			strcpy(nome_temp, (comando(buffer, nome)));
			strcpy(nome_temp2,(comando(buffer, nome2)));
			if(( grupos_temp = consultar_grupo(grupos, nome_temp))!=NULL){
				if((usuario_temp = consulta_usuario(usuarios, nome_temp2))!=NULL)
					adicionar_ao_grupo(grupos_temp, usuario_temp);
				else
					printf("pessoa nao cadastrada: %s", nome_temp2);
			}
			else
				printf("grupo nao cadastrado: %s", nome_temp);
			break;

		case 'l':

			strcpy(nome_temp, (comando(buffer, consulta)));
			if( (grupos_temp = (consultar_grupo(grupos, nome_temp)))!=NULL)
				listar_membros(grupos_temp, NULL, outro);
			else
				printf("grupo nao cadastrado: %s", nome_temp);
			break;

		case 'h':

			strcpy(nome_temp, (comando(buffer, nome)));
			strcpy(nome_temp2,(comando(buffer, nome2)));
			if(( grupos_temp = consultar_grupo(grupos, nome_temp))!=NULL){
				if((usuario_temp = consulta_usuario(usuarios, nome_temp2))!=NULL)
					remover_do_grupo(grupos_temp, usuario_temp);
				else
					printf("pessoa nao cadastrada: %s", nome_temp2);
			}
			else
				printf("grupo nao cadastrado: %s", nome_temp);
			break;

		case 's':

			strcpy(nome_temp, (comando(buffer, consulta)));
			if( (consultar_grupo(grupos, nome_temp))!=NULL)
				remover_grupo(grupos, nome_temp, usuarios);
			else
				printf("grupo nao cadastrado: %s", nome_temp);
			break;

		case 'u':

			strcpy(nome_temp, (comando(buffer, nome)));
			strcpy(nome_temp2,(comando(buffer, nome2)));
			if(( grupos_temp = consultar_grupo(grupos, nome_temp))!=NULL){
				if(( grupos_temp2 = consultar_grupo(grupos, nome_temp2))!=NULL)
					unir_grupos(grupos_temp, grupos_temp2, grupos, usuarios);
				else
					printf("grupo nao cadastrado: %s", nome_temp2);
			}
			else
				printf("grupo nao cadastrado: %s", nome_temp);
			break;

		case 'v':

			strcpy(nome_temp, (comando(buffer, nome)));
			strcpy(nome_temp2,(comando(buffer, nome2)));
			if((usuario_temp = consulta_usuario(usuarios, nome_temp))!=NULL){
				if((usuario_temp2 = consulta_usuario(usuarios, nome_temp2))!=NULL){
					if(comparar_grupos(usuario_temp->grupos_us, usuario_temp2->grupos_us)==NULL)
						printf("%s e %s nao pertencem ao mesmo grupo", nome_temp, nome_temp2);
					else
						printf("%s e %s pertencem ao mesmo grupo", nome_temp, nome_temp2);
				}
				else
					printf("pessoa nao cadastrada: %s", nome_temp2);
			}
			else
				printf("pessoa nao cadastrada: %s", nome_temp);
			break;

		case 'e':
			free(usuarios);	free(grupos);	free(usuario_temp);	free(usuario_temp2);
			free(grupos_temp);	free(grupos_temp2);
			break;

		default:
			break;

			}//switch

	}//while

	return 0;
}
