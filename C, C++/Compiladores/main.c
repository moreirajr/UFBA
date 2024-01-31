/* Universidade Federal da Bahia - UFBA
 * Alunos: Evaldo Machado Moreira Junior
 * 		   Renato Silva
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "analiseSintatica.h"
#include "analiseSemantica.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define BUFLEN 256
#define NUM_PALAVRAS_RESERVADAS 39


/* utilizado para pegar os tokens*/
static struct
	{	char* str;
    	int simbol;
    } palavrasReservadas[NUM_PALAVRAS_RESERVADAS] = {
    {"DECLARE",DECLARE}, {"AS",AS}, {"PUT",PUT}, {"IN",IN}, {"IF",IF}, {"THEN",THEN},{"ELSE",ELSE},
    {"FOREACH",FOREACH}, {"DO",DO}, {"FOR",FOR},{"FROM",FROM}, {"TO",TO}, {"RESIZE",RESIZE}, {"READ",READ},{"PRINT",PRINT},
    {"NUMBER",NUMBER}, {"LETTER",LETTER}, {"CHAR", CCHAR}, {"STRING", CSTRING}, {"NUM",NUM}, {"EQUAL", EQ},{"NOTEQUAL",NEQ},
    {"LESS_THAN",LT}, {"GREATER_THAN", GT}, {"LESS_T_EQ", LTE}, {"GREATER_T_EQ", GTE}, {"TIMES", TIMES}, {"PERCENT", PERCENT},
    {"PLUS", PLUS}, {"MINUS", MINUS}, {"OVER", OVER}, {"L_PARENTHESIS", LPAREN}, {"R_PARENTHESIS", RPAREN}, {"L_BRACKET", LBRACKET},
    {"R_BRACKET", RBRACKET}, {"DOT", DOT}, {"COMMA", COMMA},
    {"ENDFILE", ENDFILE}, {"ID", ID}

    };
/* Estados */
typedef enum
{ START, EST_CHAR, EST_STRING, EST_ASSIGN, EST_NUM, EST_ID, EST_LT, EST_GT, DONE }Estados;



char *imprimirToken(int);
int caractereInvalido(char c);
int isNumber(char *palavra);
char *UpperCaseT(char *pstring);


int ignorarEspaco(char c);
int cOperador(char c);
int cSeparador(char c);
int cQlinha(char c);
int cExpressao(char c);
int stringValida(char c);

char retornaProximoCaractere(void);
int validar_arquivo(char *nomeArquivo);
static void voltarCaractere(void);
int verificarToken(char *palavra);
Token analiseLexica(void);
ArvoreSintatica *arvoreSintatica;
/* ******variaveis globais********** */

long MAXSIMBOLOS;
char *simboloString;
int linhaNum = 0;
static char linhaBuf[BUFLEN]; 
static int linhaPos = 0; 
static int bufSize = 0; 
static int EOF_flag = FALSE;
int linhaErro = 1; /* numero da linha que ocorre o erro */
FILE *arquivo;
int numTokens = 1000;

/* ********************************* */

int main(int argc, char *argv[])
{
	char nomeArquivo[40];
	int n = 0, i = 1, totalTokens=0, erro = 0;
	Token *tokens = (Token*) malloc (sizeof (Token) * numTokens);
	strcpy(nomeArquivo,argv[1]);


	
	/* arquivo invalido */
	if(validar_arquivo(nomeArquivo)!=1){
		fclose(arquivo);
		return 1;
	}


	/* numero de bytes no arquivo */
	arquivo = fopen(nomeArquivo,"r");
	fseek(arquivo, 0 , SEEK_END);
	MAXSIMBOLOS = ftell(arquivo);
	rewind(arquivo);
	simboloString = (char*) malloc ( (sizeof(char) * MAXSIMBOLOS) + 1);

	/* Analise Lexica */
	while(1){
		tokens[n++] = analiseLexica();
		if(tokens[totalTokens].token==ENDFILE)
			break;
		if(n >= (i*1000) ){
			i++;
			tokens = (Token*) realloc (tokens, i * numTokens * sizeof(Token));
		} /* realocar de ix1000 a i+1 x 1000 tokens */

		//nao realizar a analise sintatica
		if(tokens[totalTokens].token == ERRO)
			erro = 1;

		totalTokens++;
	}

	/* Imprimir Tokens
	for(i=0; i < totalTokens; i++)
		printf("%s ", tokens[i].nome);
	*/

	if(erro!=1){
		if( (arvoreSintatica = analiseSintatica_SLR1(tokens, arvoreSintatica))!=NULL ){
			printf("SIM\n");
			//printf("\n\n#######ARVORE SINTATICA#######");
			//imprimirArvore(arvoreSintatica);
			if( analiseSemantica(arvoreSintatica, 0) )
				printf("SIM\n");
			else
				printf("NAO\n");
		}
		else
			printf("NAO\n");
		}

	fclose(arquivo);
	return 0;
}



/*************************************************************/


int caractereInvalido(char c){
	if(!  ( ((c>=32) && (c<=126)) || ((c==9) || (c==10)) )  ){
		printf("ARQUIVO INVALIDO!\n");
		return 1;
	}
	return 0;
}

/*************************************************************/

int stringValida(char c){
	if( (c == '\\') || (c == 't') || (c == 'n') || (c == '\'') || (c == '\"'))
		return 1;
	return 0;
}

/*************************************************************/

char retornaProximoCaractere(void){
if (!(linhaPos < bufSize)){
 	linhaNum++;
    if (fgets(linhaBuf,BUFLEN-1,arquivo))
    {
      bufSize = strlen(linhaBuf);
      linhaPos = 0;
      return linhaBuf[linhaPos++];
    }
    else
    { EOF_flag = TRUE;
      return EOF;
    }
  }
  else return linhaBuf[linhaPos++];
}

/*************************************************************/

int ignorarEspaco(char c){
	if((c == ' ') || (c == '\n') || (c == '\t')){
		if(c == '\n')
			linhaErro++;
		return 1;
	}
	return 0;
}

/*************************************************************/

int validar_arquivo(char *nomeArquivo){
	int c;

	arquivo = fopen(nomeArquivo,"r");
    if(arquivo == NULL){
	   exit(1);
	}
	while( (c = fgetc(arquivo))!=EOF )
		if(caractereInvalido(c))
			return 0;

	fclose(arquivo);
	return 1;
}

/*************************************************************/

char *UpperCaseT(char *pstring){

	int i, len = strlen(pstring);
	char *aux = (char*) malloc( sizeof( len + 1) );

	for(i=0; i < strlen(pstring); i++)
		aux[i] = toupper(pstring[i]);

	aux[len] = '\0';
	return aux;
}

/*************************************************************/

int isNumber(char *palavra){
	int i;

	for(i=0; i < strlen(palavra); i++)
		if(!isdigit(palavra[i]))
			return 0;

	return 1;
}

/*************************************************************/

int verificarToken(char *palavra){
	int i;
	char *s;

	s = UpperCaseT(palavra);

	/*printf("\nVerificar token: %s - tipo: ", palavra); */
	


	for(i=0; i < NUM_PALAVRAS_RESERVADAS; i++)
		if(strcmp(s, palavrasReservadas[i].str)==0){
			//printf("%s\n", palavrasReservadas[i].str);//teste
			return palavrasReservadas[i].simbol;
		}


	//printf("ID\n");//teste
	return ID;
}

/*************************************************************/

char *imprimirToken(int s){
	int i;

	for(i=0; i < NUM_PALAVRAS_RESERVADAS; i++)
		if(s == palavrasReservadas[i].simbol)
			return palavrasReservadas[i].str;
	return "ERRO";
}

/*************************************************************/

int cOperador(char c){
	if( (c == '+') || (c == '-') || (c == '/') || (c == '*') || (c == '%'))
		return 1;
	return 0;
}

/*************************************************************/

int cQlinha(char c){
	if( (c == '\n') ||  (c == '\t') )
		return 1;
	return 0;
}

/*************************************************************/

int cExpressao(char c){
	if ( (c == '>') || (c == '<') || (c == '=') || (c == '(') || (c == ')') )
		return 1;
	return 0;
}

/*************************************************************/

int cSeparador(char c){
	if( (c == '.') || (c == ',') || (c == '[') || (c == ']')
			|| ignorarEspaco(c) || cExpressao(c) || cOperador(c))
		return 1;
	return 0;
}

/*************************************************************/

static void voltarCaractere(void){
	if (!EOF_flag)
		linhaPos--;
	/* retorna o caractere quando necessario,
	 * exemplo: quando um id, ou numero, le um operador, sem espaco entre eles,
	 * o id/numero e identificado*/
}

/*************************************************************/

Token analiseLexica(void){
	int salvar, simboloIndex = 0,
	numDigitos = 0, numBarra = 0, numChars = 0;
	int simboloAtual;
	Token _token;
	Estados estado = START;

	/* le caractere por caractere */
	while(estado != DONE){
		int c = retornaProximoCaractere();
		//printf("%c", c);
		salvar = TRUE;
		switch(estado)
		{	 case START:
			//printf(" Estado start\n");
			if(isdigit(c)){ /* leu numero*/
				estado = EST_NUM;
				salvar = TRUE;
				simboloAtual = NUM;
			}
			else if(isalpha(c)){	/* letra a-Z */
				salvar = TRUE;
				estado = EST_ID;
				simboloAtual = ID;
			}
			else if (ignorarEspaco(c)){
				salvar = FALSE; /* ignorar espacos, tab e quebra de linha */
			}
			else
			{	/* operadores */
				estado = DONE;
				salvar = FALSE; //FALSE
				switch(c)
				{ case EOF:
					simboloAtual = ENDFILE;
					break;
				case '=':
					simboloAtual = EQ;
					break;
				case '<':
					estado = EST_LT;
					simboloAtual = LT; /* less than */
					break;
				case '>':
					estado = EST_GT;
					simboloAtual = GT; /* greater than */
					break;
				case '+':
					simboloAtual = PLUS;
					break;
				case '-':
					simboloAtual = MINUS;
					break;
				case '*':
					simboloAtual = TIMES;
					break;
				case '/':
					simboloAtual = OVER;
					break;
				case '%':
					simboloAtual = PERCENT;
					break;
				case '(':
					simboloAtual = LPAREN;
					break;
				case ')':
					simboloAtual = RPAREN;
					break;
				case '[':
					simboloAtual = LBRACKET;
					break;
				case ']':
					simboloAtual = RBRACKET;
					break;
				case '\'' : /* aspas simples */
					salvar = TRUE;
					estado = EST_CHAR;
					simboloAtual = ASPASS;
					break;
				case '\"': /* aspas duplas */
					salvar = TRUE;
					estado = EST_STRING;
					simboloAtual = ASPASD;
					break;
				case '.':
					estado = DONE;
					simboloAtual = DOT;
					break;
				case ',':
					estado = DONE;
					simboloAtual = COMMA;
					break;
				default: /* caracteres nao inclusos no switch acima (ponto '.')*/
					salvar = TRUE;
					simboloAtual = ERRO;
					break;

				} /* final do switch */
			} /* final do else */
			break; /* break case Start */


		/* NUMEROS */
		case EST_NUM:
			//printf(" Estado num\n");

			if(cSeparador(c) || (c == EOF)){
				salvar = FALSE;
				estado = DONE;
				//if(c != ','){
				voltarCaractere();
				if(c == '\n')
					linhaErro--;
				//}
			}
			/* declaracao de var comecando com numero */
			else if(isalpha(c)){
				salvar = TRUE;
				estado = EST_NUM;
				simboloAtual = ERRO;
			}
			else if(!isdigit(c)){		/* leu numero, e le algo diferente de numero */
				salvar = FALSE;
				estado = DONE;
				simboloAtual = ERRO; //NUMBER
				voltarCaractere();
				_token.token = NUM;
				_token.nome = simboloString;
				return _token;
			}
			else {
				salvar = TRUE;
				numDigitos++;
				if(numDigitos>=10)
					simboloAtual = ERRO;
			}
			break;


		/* IDS */
		case EST_ID:
			//printf(" Estado id\n");
			if(cSeparador(c) || (c == EOF)){
				salvar = FALSE;
				estado = DONE;
				voltarCaractere();
				if(c == '\n')
					linhaErro--;
			}
			else if(isdigit(c)){
				salvar = TRUE;
				simboloAtual = ID;
			}
			else if(!isalpha(c)){
				salvar = FALSE;
				estado = DONE;
				simboloAtual = ERRO; //ID
				voltarCaractere();
				_token.token = ID;
				_token.nome = simboloString;
				return _token;
				}
			break;

		/* caractere simples */
		case EST_CHAR:
			if(numBarra==1){
				if(!stringValida(c)){
					simboloAtual = ERRO;
					numChars++;
				}
				else
					simboloAtual = CCHAR;
				numBarra=0;
				numChars++;
			}
			else
			{
				if(c == '\''){
					if(numChars!=1)
						simboloAtual = ERRO;
					estado = DONE;
				}
				else if(cQlinha(c) || (c == EOF)){
					salvar = FALSE;
					simboloAtual = ERRO;
					estado = DONE;
					if(c == '\n')
						voltarCaractere();

				}
				else if(c == '\\'){
					numBarra++;
				}
				else if(!caractereInvalido(c)){
					numChars++;
					if(c == '\"')
						simboloAtual = ERRO;
					else
						simboloAtual = CCHAR;
				}
			}
			break;

		/* string 256 caracteres */
		case EST_STRING:
			if(numBarra==1){
				if(!stringValida(c)){
					simboloAtual = ERRO;
					numChars++;
				}
				else if(simboloAtual!=ERRO)
					simboloAtual = CSTRING;
				numBarra = 0;
				numChars++;
			}
			else
			{
				if(c == '\"'){
					estado = DONE;
					if(numChars>256)
						simboloAtual = ERRO;
				}
				else if(cQlinha(c) || (c == EOF)){
					salvar = FALSE;
					simboloAtual = ERRO;
					estado = DONE;
					if(c == '\n')
						voltarCaractere();
				}
				else if(c == '\\'){
					numBarra++;
				}
				else if(!caractereInvalido(c)){
					numChars++;
					if(simboloAtual!=ERRO)
						simboloAtual = CSTRING;
					if((c == '\''))
						simboloAtual = ERRO;
				}

			}
			break;

		/* LESS THAN, pegar tokens <> e <= */
		case EST_LT:
			if(c == '>')
				simboloAtual = NEQ;
			else if(c == '=')
				simboloAtual = LTE;
			else
				voltarCaractere();
			estado = DONE;
			break;

		/* GRETER THAN, pegar token >= */
		case EST_GT:
			if(c == '=')
				simboloAtual = GTE;
			else
				voltarCaractere();
			estado = DONE;
			break;

		case DONE:  /* erro */
		default:
			//printf("\nERRO (sem sentido!!!)");
			estado = DONE;
			simboloAtual = ERRO;
			break;
		}

		/* **** final do switch estado **** */

		/* copiar Token */
		if( (salvar) && (simboloIndex <= MAXSIMBOLOS) )
			simboloString[simboloIndex++] = (char) c; /* c e do tipo int*/

		if(estado == DONE){
			simboloString[simboloIndex] = '\0';
			if(simboloAtual == ID)
				simboloAtual = verificarToken(simboloString);
			/* IMPRIMIR ERRO */
			else if(simboloAtual==ERRO){
				printf("LINHA %d: %s\n", linhaErro, simboloString);
			}

			_token.nome = (char*) malloc( strlen(simboloString) * sizeof(char) );
			strcpy(_token.nome, UpperCaseT(simboloString));
			_token.token = simboloAtual;

		}

	} /* final do while */

	memset(simboloString, 0, MAXSIMBOLOS+1);
	return _token;
}
