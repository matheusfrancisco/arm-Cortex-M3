#ifndef __SA__
#define __SA__
#include "inttypes.h"


#pragma pack(1)

#define TAM_MEMORIA 8192
#define QTD_ENTRADAS 12
#define TAM_BLOCO_PONTEIRO 2




typedef struct {
	uint16_t id;
	uint16_t posicao; 
	
}MEU_FILE;

typedef uint8_t  bloco_dados[32];

typedef uint16_t  bloco_indice[16];

struct inodo {
	char status;
	char nome[8];
	bloco_dados dados_diretos;
	uint16_t indireto;
	uint16_t tam;
};

#define TAM_ENTRADA (sizeof(struct inodo))

#define INICIO_PONTEIROS ((TAM_BLOCO_PONTEIRO)+QTD_ENTRADAS*TAM_ENTRADA)

#define INICIO_DADOS (INICIO_PONTEIROS + 225*TAM_BLOCO_PONTEIRO)



void formata (void);
MEU_FILE * meu_fopen (const char *st, const char *modo);
void meu_fputc ( uint8_t valor , MEU_FILE *A );
void meu_fseek ( MEU_FILE *A, uint16_t offset );
int meu_fgetc( MEU_FILE *A );
int meu_feof (MEU_FILE *A);

#endif
