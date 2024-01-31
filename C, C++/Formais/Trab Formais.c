#define TAMANHO 100
#define TAMANHO_CADEIA 100

#include <stdio.h>
#include <string.h>

typedef struct automato{
   char estados[TAMANHO][8], alfabeto[TAMANHO][8], finais[TAMANHO][8];
   char simboloF[10], inicial[10];
   char tranSaida[TAMANHO][10], transSimbolo[TAMANHO][5], transChegada[TAMANHO][10];
   char tranSaidaFormado[2][TAMANHO][10];
   char estadoFormado[2][TAMANHO][10];
}automato;

automato a, b, M;

void inicializa();
void leEstados(automato *aut, char *cadeia);
void leAlfabeto(automato *aut, char *cadeia);
void leSimboloF(automato *aut, char *cadeia);
void leEstadoInicial(automato *aut,char *cadeia);
void leFinais(automato *aut, char *cadeia);
void leTransicoes(automato *aut, char *cadeia, int linha);
void estadosM();
void alfabetoM(automato *aut);
void inicialM();
void transicoesM();
void imprimeM();
void imprimeTransM();
int leCadeia(char *cadeia);

int main(){
	char autom[TAMANHO_CADEIA], trans[20];
	int i=0, j;
	automato *A;
	char testes[100][30];

	inicializa(&a);
	inicializa(&b);
	inicializa(&M);

	A=&a;
	for (i=0;i<2;i++)
	{
		gets(autom);
		leEstados(A,autom);
		leAlfabeto(A,autom);
		leEstadoInicial(A,autom);
		leSimboloF(A,autom);
		leFinais(A,autom);
		gets(trans);
		gets(trans);
		j = 0;
		while(strcmp(trans,"};"))
		{
			leTransicoes(A,trans,j);
			gets(trans);
			j++;
		}
		A=&b;
	}

	estadosM();
	alfabetoM(&a);
	alfabetoM(&b);
	transicoesM();
	inicialM();
	
	strcpy(trans,"automato");

	for(i=0;i<100;i++){
		strcpy(testes[i],"");
	}
	i = 0;
	do{
		gets(trans);
		if(leCadeia(trans)){
			strcat(testes[i],trans);
			strcat(testes[i]," = true");
			i++;
		}else{
			strcat(testes[i],trans);
			strcat(testes[i]," = false");
			i++;
		}
	}while(strcmp(trans,"#END")!=0);
	
	imprimeM();
	imprimeTransM();
	for(i=0;strcmp(testes[i],"")!=0;i++){
		printf("%s\n", testes[i]);
	}
	return 0;
}

void inicializa(automato *aut)
{
	int i;
	for(i=0; i<TAMANHO; i++)
	{
		aut->estados[i][0]='\0';
		aut->alfabeto[i][0]='\0';
		aut->finais[i][0]='\0';
		aut->tranSaida[i][0]='\0';
		aut->transSimbolo[i][0]='\0';
		aut->transChegada[i][0]='\0';
	}
}

void leEstados(automato *aut, char *cadeia)
{
	int chavesA=0, j=0, i;

	for(i=0; i<TAMANHO_CADEIA; i++)
	{
		if(chavesA == 2)
		{
			if(cadeia[i]!='}')
			{
				if(cadeia[i]==',')
				{
					j++;
				}else{
					char temp[2];
					temp[0] = cadeia[i];
					temp[1] = '\0';
					strcat(aut->estados[j],temp);
				}
			}else{
				break;
			}
		}
		if (cadeia[i]=='{')
		{
			chavesA++;
		}
	}
}

void leAlfabeto(automato *aut, char *cadeia)
{
	int chavesA=0, j=0, i;

	for(i=0; i<TAMANHO_CADEIA; i++)
	{
		if(chavesA == 3)
		{
			if(cadeia[i]!='}')
			{
				if(cadeia[i]==',')
				{
					j++;
				}else{
					char temp[2];
					temp[0] = cadeia[i];
					temp[1] = '\0';
					strcat(aut->alfabeto[j],temp);
				}
			}else{
				break;
			}
		}
		if (cadeia[i]=='{')
		{
			chavesA++;
		}
	}
}

void leFinais(automato *aut, char *cadeia)
{
	int chavesA=0, j=0, i;

	for(i=0; i<TAMANHO_CADEIA; i++)
	{
		if(chavesA == 4)
		{
			if(cadeia[i]!='}')
			{
				if(cadeia[i]==',')
				{
					j++;
				}else{
					char temp[2];
					temp[0] = cadeia[i];
					temp[1] = '\0';
					strcat(aut->finais[j],temp);
				}
			}else{
				break;
			}
		}
		if (cadeia[i]=='{')
		{
			chavesA++;
		}
	}
}

void leSimboloF(automato *aut, char *cadeia)
{
	int i, cont=0;

	for(i=0; i<TAMANHO_CADEIA; i++)
	{
		if (cadeia[i]=='}')
		{
			cont++;
		}
		if (cont==2)
		{
			if (cadeia[i+2]!=',')
			{
				char temp[2];
				temp[0]=cadeia[i+2];
				temp[1]='\0';
				strcat(aut->simboloF,temp);
			}else
				break;
		}
	}
}

void leEstadoInicial(automato *aut,char *cadeia)
{
	int i, cont=0, cont2=0;

	for(i=0; i<TAMANHO_CADEIA; i++)
	{
		if (cadeia[i]=='}')
		{
			cont++;
		}
		if (cont==2)
		{
			if (cadeia[i]==',')
			{
				cont2++;
			}
		}
		if (cont2==2)
		{
			if (cadeia[i+1]!=',')
			{
				char temp[2];
				temp[0]=cadeia[i+1];
				temp[1]='\0';
				strcat(aut->inicial,temp);
			}else
				break;
		}
	}
}

void leTransicoes(automato *aut, char *cadeia, int linha)
{
	int i, b=0, b2=0, b3=0;
	char temp[2];

	for(i=1; i<TAMANHO_CADEIA; i++)
	{
		if(cadeia[i]==',')
		{
			b=1;
		}
		if(b==0)
		{

			temp[0]=cadeia[i];
			temp[1]='\0';
			strcat(aut->tranSaida[linha],temp);
		}
		if(b==1)
		{
			if((cadeia[i+1]!=')')&&(b3==0))
			{
				temp[0]=cadeia[i+1];
				temp[1]='\0';
				strcat(aut->transSimbolo[linha],temp);
			}else
				b3=1;
		}
		if (cadeia[i]=='{')
		{
			b2=1;
		}
		if(b2==1)
		{
			if(cadeia[i+1]!='}')
			{
				temp[0]=cadeia[i+1];
				temp[1]='\0';
				strcat(aut->transChegada[linha],temp);
			}else
				break;
		}

	}
}

void estadosM()
{
	int i,j,k,m=0,n=0;

	for(i=0; i<TAMANHO; i++)
	{
		if(a.estados[i][0]!='\0')
		{
			for(j=0;j<TAMANHO;j++)
			{
				if(b.estados[j][0]!='\0')
				{
					strcat(M.estados[m],a.estados[i]);
					strcat(M.estados[m],b.estados[j]);
					strcpy(M.estadoFormado[0][m],a.estados[i]);
					strcpy(M.estadoFormado[1][m],b.estados[j]);
					for(k=0;k<TAMANHO;k++)
					{
						if( !strcmp(a.estados[i],a.finais[k]) || !strcmp(b.estados[j],b.finais[k]) )
						{
							strcpy(M.finais[n],M.estados[m]);
							n++;
							break;
						}
					}
					m++;
				}
			}
		}else
			break;
	}
}

void alfabetoM(automato *aut)
{
	int i,j,m=0,b=0;

	for(i=0; i<TAMANHO; i++)
	{
		if(aut->alfabeto[i][0]!='\0')
		{
			for(j=0;j<TAMANHO;j++)
			{
				if(!strcmp(M.alfabeto[j],aut->alfabeto[i]))
				{
					b=1;
					break;
				}
			}
			if(b==0)
			{
				strcpy(M.alfabeto[m],aut->alfabeto[i]);
				m++;
			}
		}else
			break;
		b=0;
	}
}

void transicoesM()
{
	int i,j,k,m=0;

	for (i=0; i<TAMANHO; i++)
	{
		if(M.estados[i][0]!='\0')
		{
			for(j=0;j<TAMANHO;j++)
			{
				if(M.alfabeto[j][0]!='\0')
				{
					strcpy(M.tranSaida[m],M.estados[i]);
					strcpy(M.tranSaidaFormado[0][m],M.estadoFormado[0][i]);
					strcpy(M.tranSaidaFormado[1][m],M.estadoFormado[1][i]);
					strcpy(M.transSimbolo[m],M.alfabeto[j]);
					m++;
				}else
					break;
			}
		}else
			break;
	}
	for(i=0;i<TAMANHO;i++)
	{
		if(M.tranSaida[i]!='\0')
		{
			for(j=0;j<TAMANHO;j++) // procura o estado no primeiro automato
			{
				//if(strstr(M.tranSaida[i],a.tranSaida[j]))
				if(strcmp(M.tranSaidaFormado[0][i],a.tranSaida[j])== 0)
				{
					for(k=0;k<TAMANHO;k++) // procura o estado no segundo automato
					{
						//if(strstr(M.tranSaida[i],b.tranSaida[k]))
						if(strcmp(M.tranSaidaFormado[1][i],b.tranSaida[k]) == 0)
						{
							if(strcmp(M.transSimbolo[i],a.transSimbolo[j])==0 && strcmp(M.transSimbolo[i],b.transSimbolo[k])==0 )
							{
								strcat(M.transChegada[i],a.transChegada[j]);
								strcat(M.transChegada[i],b.transChegada[k]);
							}
						}
					}
				}
			}
		}else
			break;
	}
}

void inicialM()
{
	strcat(M.inicial,a.inicial);
	strcat(M.inicial,b.inicial);
}

void imprimeM()
{
	int i;

	printf("M = {{");
	for(i=0;i<TAMANHO;i++)
	{
		if(M.estados[i][0]!='\0')
		{
			printf("%s", M.estados[i]);
			if(M.estados[i+1][0]!='\0')
			{
				printf(",");
			}else
			{
				printf("},{");
			}
		}
	}
	for(i=0;i<TAMANHO;i++)
	{
		if(M.alfabeto[i][0]!='\0')
		{
			printf("%s", M.alfabeto[i]);
			if(M.alfabeto[i+1][0]!='\0')
			{
				printf(",");
			}else
			{
				printf("},f,");
			}
		}
	}
	printf("%s",M.inicial);
	printf(",{");
	for(i=0;i<TAMANHO;i++)
	{
		if(M.finais[i][0]!='\0')
		{
			printf("%s", M.finais[i]);
			if(M.finais[i+1][0]!='\0')
			{
				printf(",");
			}else
			{
				printf("}}");
			}
		}
	}
}

void imprimeTransM()
{
	int i;

	printf("\nf={\n");
	for(i=0;i<TAMANHO;i++)
	{
		if(M.tranSaida[i][0]!='\0')
			printf("%c%s%c%s%s%s%c\n",'(',M.tranSaida[i],',',M.transSimbolo[i],")={",M.transChegada[i],'}');
	}
	printf("};\n");
}

int leCadeia(char *cadeia)
{
	int i,j,achou_final=0,achou=0,posLetr=0;
	char estado[10];

	strcpy(estado,M.inicial);

	for(;;)
	{

		achou = 0;
		for(i=0;i<TAMANHO;i++)
		{
			if(!strcmp(estado,M.tranSaida[i]))
			{
				char temp[2];

				temp[0] = cadeia[posLetr];
				temp[1] = '\0';
				if(strcmp(temp,M.transSimbolo[i]) == 0)
				{
					strcpy(estado,M.transChegada[i]);
					if(cadeia[posLetr+1]=='\0')
					{
						for(j=0;j<TAMANHO;j++)
						{
							if(!strcmp(M.transChegada[i],M.finais[j]))
							{
								achou_final = 1;
							}
						}
						if (achou_final==1)
							return 1;
						else
							return 0;
					}
					posLetr++;
					achou = 1;
					break;

				}
			}
		}
		if(achou == 0){
			return 0;
		}
	}
}
