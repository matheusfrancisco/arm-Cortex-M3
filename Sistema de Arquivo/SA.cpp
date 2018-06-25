
#include "SA.h"
#include "memoria.h"
#include "inttypes.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>


//uint16_t aloca (void);
//void escreva_bloco_dados(uint16_t numero ,uint8_t *valor);
//void leia_bloco_dados(uint16_t numero, uint8_t *valor);



/**
 * MEU_format() - Format de filesystem
 *
 * @description
 * 		Formats the entire filesystem
 * @param
 *
 * @return
 *
 */
void formata (void)
{

	/*pra formatar escrevemos o cabecalho como
	0 e criamos as 12 entradas, depois cria a 
	lista de bloco (ponteiros), e depois os blocos de dados */
	memoria.init();
	escreve_cabecalho(0);
	for (int x=0;x<12;x++) cria_entrada(x);
	cria_blocos_livres();
	
	cria_blocos_dados();
	
}




void le_entrada_arquivo (uint16_t numero, 	struct inodo *tmp)
{
	uint16_t posicao = 2 + numero*sizeof(struct inodo);
	memoria.read(posicao, sizeof(struct inodo), (uint8_t *) tmp);
}
void escreve_entrada( uint16_t numero , struct inodo tmp)
{
	uint16_t posicao = 2 + numero*sizeof(struct inodo);
	memoria.write(posicao, sizeof(struct inodo), (uint8_t *)&tmp);
}
	


MEU_FILE * meu_fopen (const char *st, const char *modo)
{
	MEU_FILE * tmp = NULL;
	
	struct inodo meu_inodo;
	
	/*Tenta abrir ou criar o arquivo
	se conseguir alocar com malloc retorna
	se não retorna NULL*/
	if (strcmp(modo, "w")==0)
	{
		int livre = -1;
		for (int x=0;x<12;x++)
		{
			
			le_entrada_arquivo(x, &meu_inodo);
			
			/* se o inodo lido estive desocupado, 
			e a var livre for -1 marca essa posição
			 como a posição para criar o arquivo.*/

			if ((meu_inodo.status==0) && (livre==-1)) livre = x;
			if (strcmp(meu_inodo.nome, st)==0)
			{
					// trabalho pois precisa desalocar blocos
				remove_entrada(x);
				/** Se não achar entrada livre
				vamos colcoar o mesmo nome na entrada*/
				if(livre == -1)
					livre =x;
					
			}
		}
		/*Falta fazer se não tiver nenhuma vamos retornar nulo */

		meu_inodo.status = 1;
		meu_inodo.tam=0;
		strcpy(meu_inodo.nome,st);
		meu_inodo.indireto=0xffff;
		escreve_entrada( livre , meu_inodo );		
	
		tmp  = (MEU_FILE *) malloc (sizeof(MEU_FILE));
		tmp->id = livre;
		tmp->posicao = 0;
		
		
	}
	return tmp;
}

/*Inicio do Cabeçalho*/

uint16_t le_cabecalho ()
{
	/*retorna o valor do cabeçalho 
	Quer dizer o valor do próximo bloco disponivel para alocação*/
	uint16_t numero;
	memoria.read(0, 2, (uint8_t *) &numero);
	return numero;
}
void escreve_cabecalho (uint16_t numero)
{
	/*/grava o numero do proximo bloco disponível no cabeçalho.*/
	memoria.write(0, 2, (uint8_t *) &numero);
}

/*Fim do Cabeçalho*/

void leia_entrada (uint8_t numero, struct inodo *i)
{
	/* pula o cabecalho (2) e faz o numero da entrada vezes o
	 tamanho, isso retorna a posição na memória da entrada.*/
	uint16_t posicao = 2 + numero*sizeof(struct inodo);
	memoria.read(posicao, sizeof(struct inodo), (uint8_t *)i);  //le a entrada
}

void escreva_entrada (uint8_t numero, struct inodo *i)
{
	/* pula o cabecalho (2) e faz o numero da entrada vezes o
	 tamanho, isso retorna a posição na memória da entrada.*/
	uint16_t posicao = 2 + numero*sizeof(struct inodo);
	memoria.write(posicao, sizeof(struct inodo), (uint8_t *)i);// grava a entrada
}

void meu_fseek ( MEU_FILE *A, uint16_t offset )
{
	A->posicao = offset;
}

int meu_fgetc( MEU_FILE *A )
{
	struct inodo lida;
	bloco_indice bloco_idx;
	bloco_dados  bloco;
	uint16_t n;
	uint8_t valor;
	
	
	if (A == NULL) return 0;
	uint16_t id = A->id;
	uint16_t posicao = A->posicao;
	
		
	leia_entrada(id, &lida);

	if (posicao < 32)
	{
		valor = lida.dados_diretos[posicao];
		
	}
	else  
	{	
		if (lida.indireto==0xFFFF) 
		{
			return 0;
		}
		uint16_t numero_entrada = posicao / 32;
		uint8_t deslocamento    = posicao % 32;
		numero_entrada--;
		
		
		leia_bloco_dados (lida.indireto, (uint8_t *) &bloco_idx);
		if (bloco_idx[numero_entrada]==0xffff)
		{
			return 0;
		}
		leia_bloco_dados (lida.indireto, (uint8_t *) &bloco_idx);
		n = bloco_idx[numero_entrada];
		
		leia_bloco_dados (n, (uint8_t *) &bloco);
		valor = bloco[deslocamento];

		 
		
	}
	
	A->posicao++;
	return valor;
}

int meu_feof (MEU_FILE *A)
{
	struct inodo lida;
	if (A == NULL) return 1;
	uint16_t id = A->id;

	
		
	leia_entrada(id, &lida);
	
	if (lida.tam > A->posicao) return 0;
	return 1;
	
}

/**
 * MEU_fputc() - 
 *
 * @description
 * 		Writes the byte c.
 *		escreve um byte.
 * @param
 * 		Passa o arquivo e o valor
 * 		ile and to value
 * @return
 * 	
 */
void meu_fputc ( uint8_t valor , MEU_FILE *A )
{
	struct inodo lida;
	bloco_indice bloco_idx;
	bloco_dados  bloco;
	uint16_t n;
	
	
	if (A == NULL) return;
	uint16_t id = A->id;
	uint16_t posicao = A->posicao;
	
		
	leia_entrada(id, &lida);

	if (posicao < 32)
	{
	/*se a posição corrente for < 32 
	o valor a ser escrito é no dado direto*/
		lida.dados_diretos[posicao] = valor;
		
	}
	else  
	{	
		/*Se não é em indireto*/
		if (lida.indireto==0xFFFF) 
		{
			/*caso não foi alocado, devemos alocar o indireto*/
			lida.indireto = aloca ();
		}
		/*Calcula deslocamento e entrada corrente*/
		uint16_t numero_entrada = posicao / 32;
		uint8_t deslocamento    = posicao % 32;
		numero_entrada--;
		
		
		leia_bloco_dados (lida.indireto, (uint8_t *) &bloco_idx);
		/*Caso a entrada não esteja alocada temos que alocar ela*/
		if (bloco_idx[numero_entrada]==0xffff)
		{
			/*le a entra em questão*/
			bloco_idx[numero_entrada] = aloca();
			/*marca o bloco*/
			escreva_bloco_dados(lida.indireto , (uint8_t *) bloco_idx);
		}
		/*le o bloco de dados*/
		leia_bloco_dados (lida.indireto, (uint8_t *) &bloco_idx);
		/*bloco da entrada*/
		n = bloco_idx[numero_entrada];
		
		leia_bloco_dados (n, (uint8_t *) &bloco);//vai ler o  bloco
		bloco[deslocamento] = valor;
		escreva_bloco_dados(n ,  (uint8_t *) &bloco);
		 
		
	}
	/*Aqui esta incrmentando o tamanho*/
	if (A->posicao >= lida.tam) lida.tam++;
	/*Aqui esta incrementando a posição*/
	A->posicao++;
	/*Aqui reescreve a entrada*/
	escreva_entrada(id, &lida);
	
}

void cria_entrada (uint8_t numero)
{

	struct inodo tmp;	//Cria lista
	tmp.status = 0; // define seus valores padrões como 0
	tmp.tam=0;
	strcpy(tmp.nome,"VAZIO"); // Nome fica vazio
	tmp.indireto=0xFFFF;
	for (int a=0;a<32;a++) tmp.dados_diretos[a]=0;// zera os dados diretos
	/*pula o cabecalho (2) e faz o numero da entrada vezes o tamanho,
	 isso retorna a posição na memória da entrada.*/
	uint16_t posicao = 2 + numero*sizeof(struct inodo);
	memoria.write(posicao, sizeof(struct inodo), (uint8_t *)&tmp); //grava na memoria
	
}
/*Inicio Bloco de ponteiros*/
void cria_blocos_livres()
{
	/*bloco livre 
	lista de blocos para alocação*/
	for (int x=0;x<224;x++)
		/*para cada lista, fizemos cada bloco i, receer i+1*/
		escreva_bloco_ponteiro(x, x+1);
		/*Os blocos da lista tem um ponteiro seg (next->)
		para o proximo da lista caso estivermos no ultimo bloco
		marcamos  o next como inválido*/
	escreva_bloco_ponteiro(224, 0xFFFF);
}

void escreva_bloco_ponteiro(uint16_t numero, uint16_t valor)
{
	/*calcula a posição na memoria 
	do bloco para alocação (ponteiro) (da lista) em questão*/
	uint16_t endereco = numero * 2 + INICIO_PONTEIROS; 
	memoria.write(endereco, 2, (uint8_t *)&valor);
}
void leia_bloco_ponteiro(uint16_t numero, uint16_t *valor)
{
	/*calcula a posição na memoria 
	do bloco para alocação (ponteiro) (da lista) em questão*/
	uint16_t endereco = numero * 2 + INICIO_PONTEIROS; 
	memoria.read(endereco, 2, (uint8_t *)valor);
}

/*Fim dos Bloco de ponteiros*/


/*Inicio dos blocos de dados*/
uint16_t aloca (void)
{
	bloco_indice  bloco_idx;
	 
	uint16_t lido;
	uint16_t  novo_bloco = le_cabecalho();
	// le o bloco de dados no lido
	
	leia_bloco_ponteiro(novo_bloco, &lido);
	
	//escreve no cabecalho o next do bloco
	
	escreve_cabecalho (lido);
	
	leia_bloco_dados (novo_bloco, (uint8_t *) &bloco_idx);
	for (int x=0;x<16;x++) //marca todos os valores do blcoo como invalido 
		bloco_idx[x]=0xffff;
	escreva_bloco_dados(novo_bloco, (uint8_t *) &bloco_idx);//escreve o bloco novamente

	
	
	return novo_bloco;
}


void cria_blocos_dados()
{
	uint8_t buffer[32];
	
	for (int x=0;x<32;x++) buffer[x]=0;
	
	for (int x=0;x<225;x++)
		escreva_bloco_dados(x, buffer);
	

}

void escreva_bloco_dados(uint16_t numero ,uint8_t *valor)
{
		uint16_t endereco = numero * sizeof(bloco_dados) + INICIO_DADOS; 
		memoria.write(endereco,  sizeof(bloco_dados),  valor);
}

void leia_bloco_dados(uint16_t numero ,uint8_t *valor)
{
		uint16_t endereco = numero * sizeof(bloco_dados) + INICIO_DADOS; 
		memoria.read(endereco, sizeof(bloco_dados),  valor);
}

/*Fim dos blocos de dados*/


