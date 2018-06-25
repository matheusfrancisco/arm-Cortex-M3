#ifndef __SA__
#define __SA__
#include "inttypes.h"


#pragma pack(1)

#define TAM_MEMORIA 8192 /*Tamanho da memória*/
#define QTD_ENTRADAS 12 /*quantidade arquivos com máximo de tamanho 17*32 = 544Bytes de dados*/
#define TAM_BLOCO_PONTEIRO 2 /* Lista para apontar para um quadradinho de 2Bytes*/

// Implementado 
#define TAM_ENTRADA (sizeof(struct inodo)) // é o tamanho do inodo
#define INICIO_PONTEIROS ((TAM_BLOCO_PONTEIRO)+QTD_ENTRADAS*TAM_ENTRADA) //define a posição da memória que começa a lista de blocos (ponteiros)
 //é o tamanho do vetor quando se lê um bloco de dados
#define TAMANHO_BLOCO_DADOS 32
//é o tamanho do vetor quando se lê um bloco de endereços 
#define TAMANHO_BLOCO_INDICES 16 
#define TAM_ENTRADA (sizeof(struct inodo))
 //define a posição da memória que começa a lista de blocos (dos dados)
#define INICIO_PONTEIROS ((TAM_BLOCO_PONTEIRO)+QTD_ENTRADAS*TAM_ENTRADA) 
//define a posição da memória que começa a lista de blocos (dos dados)
#define INICIO_DADOS (INICIO_PONTEIROS + 225*TAM_BLOCO_PONTEIRO)




typedef struct {
	uint16_t id;
	uint16_t posicao; // posição dentro do arquivo
	char modo;
}MEU_FILE;

typedef uint8_t  bloco_dados[TAMANHO_BLOCO_DADOS];

typedef uint16_t  bloco_indice[TAMANHO_BLOCO_INDICES];

struct inodo {
	char status;
	char nome[8];
	bloco_dados dados_diretos;
	uint16_t indireto;
	uint16_t tam;
};

/*Feitas em sala de aula*/
uint16_t aloca (void);
void le_entrada_arquivo (uint16_t numero, 	struct inodo *tmp);
void escreve_entrada( uint16_t numero , struct inodo tmp);
void escreve_cabecalho (uint16_t numero);;
uint16_t le_cabecalho ();
void leia_entrada (uint8_t numero, struct inodo *i);
void escreva_entrada (uint8_t numero, struct inodo *i);
void cria_entrada (uint8_t numero);
void escreva_bloco_ponteiro(uint16_t numero, uint16_t valor);
void leia_bloco_ponteiro(uint16_t numero, uint16_t *valor);
void escreva_bloco_dados(uint16_t numero ,uint8_t *valor);
void leia_bloco_dados(uint16_t numero ,uint8_t *valor);
void cria_blocos_livres();
void cria_blocos_dados();
void formata (void);

/*END Feitas em sala de aula*/


/*Abertura e fechamento de arquivo*/
MEU_FILE * meu_fopen (const char *st, const char *modo);
int meu_fclose(MEU_FILE *A);

/*Passa o bit e o arquivo a ser escrito*/
void meu_fputc ( uint8_t valor , MEU_FILE *A );

/**/
void meu_fseek ( MEU_FILE *A, uint16_t offset );
/**/
int meu_fgetc( MEU_FILE *A );
/**/
int meu_feof (MEU_FILE *A);
 

/*Função formata SA*/
void meu_fomartar();





#endif
