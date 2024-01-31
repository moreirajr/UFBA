/*
 *
 */

#ifndef TIPOS_H_
#define TIPOS_H_


typedef struct {
	char nome[21];
	struct Amigos *prox;
}Amigos;

typedef struct Lista_dos_Grupos	{	//estrutura dos grupos
	char nome[21];
	struct Grupos_Amigos *prox;
	Amigos *membros;
}Grupos_Amigos;


typedef struct Dados_do_usuario{	//estrutura dos usuarios
	char nome[21];
	int idade;
	Grupos_Amigos *grupos_us;
	struct User_data *prox;
	struct User_data *ant;
	Amigos *amigos_us;
	int qnt_grupos;
}User_data;

typedef enum tipos {consulta, nome, nome2, idade, outro} TIPO;
//cadastro para nome seguido de idade (p 'nome' 'idade'), consulta para apenas nome (c 'nome')


/* Prototipo das funcoes*/
void bzero_macro(char *string, int lenght);	//limpar string
char *comando(char *comando, TIPO t);		//reconhecer os comandos

User_data *cadastrar_usuario(User_data *u, char *nome_temp, int idade);
User_data* consulta_usuario(User_data *u, char *nome);
User_data *remove_usuario(User_data *u, User_data *remover);

User_data *adicionar_amigo(User_data *u, User_data *u2, User_data *usuarios);
User_data *remove_amigo(User_data *u, User_data *u1, User_data *u2);
void listar_amigos(User_data *u, char *nome);

Grupos_Amigos *criar_grupos(Grupos_Amigos *g, char *nome);
Grupos_Amigos *remover_grupo(Grupos_Amigos *g, char*nome, User_data *usuarios);
Grupos_Amigos *consultar_grupo(Grupos_Amigos *g, char *nome);
int listar_membros(Grupos_Amigos *g, char *nome, TIPO t);

Grupos_Amigos *adicionar_ao_grupo(Grupos_Amigos *g, User_data *usuario);
Grupos_Amigos *remover_do_grupo(Grupos_Amigos *g, User_data *usuario);

Grupos_Amigos *unir_grupos(Grupos_Amigos *g, Grupos_Amigos *g2, Grupos_Amigos *grupos, User_data *u);
Grupos_Amigos *comparar_grupos(Grupos_Amigos *g, Grupos_Amigos *g2);

#endif /* TIPOS_H_ */
