
#include "SA.h"
#include "memoria.h"
#include "inttypes.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>


//uint16_t aloca (void);
//void escreva_bloco_dados(uint16_t numero ,uint8_t *valor);
//void leia_bloco_dados(uint16_t numero, uint8_t *valor);



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

void remove_entrada (uint8_t id){
	struct inodo tmp;
	
	leia_entrada(id, &tmp); // le a entrada desejada e salva no tmp
	
	uint16_t ponteiro_indireto = tmp.indireto;
	if(ponteiro_indireto != 0xFFFF){ //se tiver dados indiretos alocados.
	
		bloco_indice  bloco_idx;  //bloco de endereços
		leia_bloco_dados (ponteiro_indireto, (uint8_t *) &bloco_idx); //le o bloco do indireto (que tem os endereços dos blocos de dados)
		
		for (int i = 0; i < 16 && bloco_idx[i] != 0xffff; i++) // remove os blocos (referenciadas no bloco do indireto) ate achar uma entrada inválida.
		{
			escreva_bloco_ponteiro(bloco_idx[i], le_cabecalho());
			escreve_cabecalho(bloco_idx[i]); // para remover uma entrada, gravamos o valor do cabeçalho no bloco referente a entrada em questão, e marcamos essa como o valor do cabeçalho. (novo.valor = cabecalho.valor; cabecalho.valor = novo; +- assim);
		}
		escreva_bloco_ponteiro(ponteiro_indireto, le_cabecalho());
		escreve_cabecalho(ponteiro_indireto); //faz a mesma coisa, só que agora para o bloco do indireto
	}
	
	cria_entrada(id); //recria a entrada, (zera tudo)
}


	
/*Inicio da leitura e escrita*/
/**
 * MEU_fopen() - Open file
 *
 * @description
 * 		Opens the file whose name is the string pointed to by path and associates a file with it.
 *
 * 		The argument mode points to a string beginning with one of the following sequences
 * 		(possibly followed by additional characters, as described below):
 *
 * 	r	Open  for reading.  The file is positioned at the
 *              beginning of the file.
 * 	w       Open  for  reading and writing.  The file is created if it does
 *              not exist, otherwise it is truncated.  The file is positioned
 *              at the beginning of the file.
 * @param
 * 		nome:	Filename
 * 		modo:   Operation mode
 * @return
 * 		Return the character written as an unsigned char cast to an int or EOF on error
 */
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

			if ((meu_inodo.status==0) && (livre==-1)) 
			{
				livre = x;
			}
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
		/*cria um inodo novo e marca como ocupado*/
		meu_inodo.indireto=0xffff; 
		/*grava na memoria*/
		escreve_entrada( livre , meu_inodo );		
		
		//aloca o arquvio que será retornado, arquivo != inodo.
		tmp  = (MEU_FILE *) malloc (sizeof(MEU_FILE));
		tmp->id = livre;
		tmp->posicao = 0;
		
		
	}
	else if (strcmp(modo, "r+") ==0){
		/*Implementar modo r+*/
		int livre =-1; /*Se for modo r+*/
		/*procuramos a entrada desejada*/
		for (int x  =0; x< 12; x++)
		{
			leia_entrada(x,&meu_inodo);
			if (strcmp(meu_inodo.nome, st)==0)
			{
				livre=x;
				 break;
			}
		}
		/*Fazer retornar NULL se não existir o arquivo*/
		if(livre==-1){
			return NULL;
		}
		/*se existir, retorna o arquivo.*/
		else{
			tmp = (MEU_FILE *) malloc (sizeof(MEU_FILE));
			tmp->id = livre;
			tmp->posicao = 0;
		}
	}
	return tmp;
}


/**
 * MEU_fclose() - Close File
 *
 * @description
 * 		
 *
 *
 * @param
 *
 * @return
 *
 */
int meu_fclose(MEU_FILE *A)
{
	if(A == NULL){
		return 1;
	}
	/*Acho que funciona faz uns testes ai*/
	free(A);
	return 0;

}

/*FIM da abertura  e fechamento*/



/** Inicio da manipulação de Arquivo **/
/**
 * MEU_fseek() - Reposition stream position indicator
 *
 * @descriptionS
 * 		
 *
 * @param
 * 		
 * @return
 * 		
 */
void meu_fseek ( MEU_FILE *A, uint16_t offset )
{
	A->posicao = offset;
	/*Falta implementar algo */
}

/** buffer de bytes a serem escrito
tamanho é o tamanho de cada item, e 
count é a quantidade a ser escrita
**/
uint16_t meu_fwrite(MEU_FILE *A, uint8_t * buffer, uint16_t size, uint16_t count) {

    uint16_t qn_esccrito = 0; // é a quantidade de itens que vai ser lido

    // quantidade de bytes que falta escrever
    uint16_t qtd_escrever = size * count;

	//uint8_t P_MEUFILE = A->posicao;//
	uint16_t id = A->id;
	uint16_t posicao = A->posicao;

	uint16_t n;
	/** Para lembrar essa struct eu consigo acessar direto e indireto
	char status;
	char nome[8];
	bloco_dados dados_diretos;
	uint16_t indireto;
	uint16_t tam;
	**/
    struct inodo inodo_lida; 

	bloco_dados bloco_idx;
	bloco_dados bloco;

	if(buffer == NULL || A == NULL)
	{
		//ERRROOO
		return -2; 
	}
	if(A->modo == 'r'){
		return 0;
	}
	if(count ==0)
	{
		return 0;
	}
	leia_entrada(id, &inodo_lida);// le a entrada do arquivo em questão
	


	if (posicao < 32) {
		/*TEM quer fazer um for 
		que a quantidade escrita vai ser menor que a posicao menos 32
		e a quantidade que vai escrever tem que ser maior que zero*/

		printf("qn escrito = %i, qtd escreve = %i", qn_esccrito, qtd_escrever);
		while((A->posicao+qn_esccrito) <  32 && qtd_escrever > 0) {
			
			// aloca no bloco direto a posicao do buffer
			printf("@@escrevendo %i @@\n\n ", buffer[qn_esccrito]);
			inodo_lida.dados_diretos[posicao] = buffer[qn_esccrito];
			qn_esccrito++;
			// decrementa 1 byte
			qtd_escrever--;
			posicao++;

			// aumenta tamanho do inodo
			inodo_lida.tam++;
		}
		
	}

	/*se for ler ainda e tem q verificar se tem algo no indireto e alocar*/  
	// verifica se chegou no final e tem algo p escrever
	if (inodo_lida.indireto==0xFFFF && qtd_escrever > 0)  {
		/*caso não foi alocado, devemos alocar o indireto*/
		inodo_lida.indireto = aloca ();
	}


	/*Calcula deslocamento e entrada corrente*/
	// dos blocos de endereço seja a inicial
	uint16_t numero_entrada = (posicao / 32);
	// calcula o deslocamento caso a poosição 
	uint8_t deslocamento    = posicao % 32;
	
	// aloca bloco de dados indireto p começar a operacao
	leia_bloco_dados (inodo_lida.indireto, (uint8_t *) &bloco_idx);
	/*Caso a entrada não esteja alocada temos que alocar ela*/
	if (bloco_idx[numero_entrada]==0xffff)
	{
		/*le a entra em questão*/
		bloco_idx[numero_entrada] = aloca();
		/*marca o bloco*/
		escreva_bloco_dados(inodo_lida.indireto , (uint8_t *) bloco_idx);
	}
	/*le o bloco de dados*/
	leia_bloco_dados (inodo_lida.indireto, (uint8_t *) &bloco_idx);
	
	/*bloco da entrada*/
	//enquanto tiver coisa pra escrever e tiver endereço no bloco
	// cuidar o overhead
	// tem que ter o que escrever > 0 e entrada tem que ser menor
	// q o numero de blocos
	
	while(qtd_escrever > 0 && numero_entrada < TAMANHO_BLOCO_INDICES) {
		// se for invalido tem q alocar
		if(bloco_idx[numero_entrada] =0xffff) {
			bloco_idx[numero_entrada] = aloca();
			escreva_bloco_dados(inodo_lida.indireto , (uint8_t *) bloco_idx); 
			// vai atulizar o ponteiro
		}
		//agr le pq ta atualizado
		leia_bloco_dados (inodo_lida.indireto, (uint8_t *) &bloco_idx); 
		n= bloco_idx[numero_entrada];
		leia_bloco_dados (n, (uint8_t *) &bloco);//vai ler o  bloco
			// tem que fazer um for pra gravar tipo o do fputc
		// que incrementa o tamnho e faz um deslocamento	
		// nesse for é gravado em um bloco os 
		//valores do vetor passado como parâmetro.
		int i = deslocamento;
		
		/*
		while(qtd_escrever > 0  &&  i < 32 ) {
			bloco[i] = buffer[qn_esccrito];
			qtd_escrever--;
			qn_esccrito++;
			inodo_lida.tam++;
			posicao++;
			i++;		
		}*/

		for(uint16_t x = deslocamento; x < 32 && qtd_escrever > 0; x++) {
			bloco[x] = buffer[qn_esccrito++];
			qtd_escrever--;
			posicao++;
			inodo_lida.tam++;
		}

		// escreve novamente o bloco que acabou de ser atualizado(bloco de dados)
		escreva_bloco_dados(n ,  (uint8_t *) &bloco);
		// zera o deslocamento para o próximo bloco
		deslocamento = 0;
		numero_entrada++;
	}
	A->posicao = posicao;
	escreva_entrada(id, &inodo_lida);
	return qn_esccrito;
}
	

// Lê um arquivo e passa count bytes para o buffer desejado
uint16_t meu_fread(MEU_FILE *A, uint8_t * buffer, uint16_t size, uint16_t count) {
    uint16_t qn_lido = 0; // é a quantidade de itens que vai ser lido

    // quantidade de bytes que falta escrever
    uint16_t qtd_escrever = size * count;

	//uint8_t P_MEUFILE = A->posicao;//
	uint16_t id = A->id;
	uint16_t posicao = A->posicao;

	uint16_t n;
	/** Para lembrar essa struct eu consigo acessar direto e indireto
	char status;
	char nome[8];
	bloco_dados dados_diretos;
	uint16_t indireto;
	uint16_t tam;
	**/
    struct inodo inodo_lida; 

	bloco_dados bloco_idx;
	bloco_dados bloco;

	if(buffer == NULL || A == NULL)
	{
		//ERRROOO
		return -2; 
	}
	if(count ==0)
	{
		return 0;
	}
	leia_entrada(id, &inodo_lida);// le a entrada do arquivo em questão
	


	if (posicao < 32) {
		/*TEM quer fazer um for 
		que a quantidade escrita vai ser menor que a posicao menos 32
		e a quantidade que vai escrever tem que ser maior que zero*/

		while((A->posicao+qn_lido) <  32 && qtd_escrever > 0) {
			buffer[qn_lido] = inodo_lida.dados_diretos[posicao];
			qn_lido++;
			// decrementa 1 byte
			qtd_escrever--;
			posicao++;
		}
		
	}

	/*se for ler ainda e tem q verificar se tem algo no indireto e alocar*/  
	// verifica se chegou no final e tem algo p escrever
	if (inodo_lida.indireto==0xFFFF && qtd_escrever > 0)  {
		/*caso não foi alocado, devemos alocar o indireto*/
		inodo_lida.indireto = aloca ();
	}


	/*Calcula deslocamento e entrada corrente*/
	// dos blocos de endereço seja a inicial
	uint16_t numero_entrada = (posicao / 32);
	// calcula o deslocamento caso a poosição 
	uint8_t deslocamento    = posicao % 32;
	
	// aloca bloco de dados indireto p começar a operacao
	leia_bloco_dados (inodo_lida.indireto, (uint8_t *) &bloco_idx);
	/*Caso a entrada não esteja alocada temos que alocar ela*/
	if (bloco_idx[numero_entrada]==0xffff)
	{
		/*le a entra em questão*/
		bloco_idx[numero_entrada] = aloca();
		/*marca o bloco*/
		escreva_bloco_dados(inodo_lida.indireto , (uint8_t *) bloco_idx);
	}
	/*le o bloco de dados*/
	leia_bloco_dados (inodo_lida.indireto, (uint8_t *) &bloco_idx);
	
	/*bloco da entrada*/
	//enquanto tiver coisa pra escrever e tiver endereço no bloco
	// cuidar o overhead
	// tem que ter o que escrever > 0 e entrada tem que ser menor
	// q o numero de blocos
	
	while(qtd_escrever > 0 && numero_entrada < TAMANHO_BLOCO_INDICES) {
		// se for invalido tem q alocar
		if(bloco_idx[numero_entrada] =0xffff) {
			bloco_idx[numero_entrada] = aloca();
			escreva_bloco_dados(inodo_lida.indireto , (uint8_t *) bloco_idx); 
			// vai atulizar o ponteiro
		}
		//agr le pq ta atualizado
		leia_bloco_dados (inodo_lida.indireto, (uint8_t *) &bloco_idx); 
		n= bloco_idx[numero_entrada];
		leia_bloco_dados (n, (uint8_t *) &bloco);//vai ler o  bloco
			// tem que fazer um for pra gravar tipo o do fputc
		// que incrementa o tamnho e faz um deslocamento	
		// nesse for é gravado em um bloco os 
		//valores do vetor passado como parâmetro.
		int i = deslocamento;
		
		/*
		while(qtd_escrever > 0  &&  i < 32 ) {
			bloco[i] = buffer[qn_esccrito];
			qtd_escrever--;
			qn_esccrito++;
			inodo_lida.tam++;
			posicao++;
			i++;		
		}*/

		for(uint16_t x = deslocamento; x < 32 && qtd_escrever > 0; x++) {
			buffer[qn_lido] = bloco[x];
			qn_lido++;
			qtd_escrever--;
			posicao++;
		}

		// escreve novamente o bloco que acabou de ser atualizado(bloco de dados)
		escreva_bloco_dados(n ,  (uint8_t *) &bloco);
		// zera o deslocamento para o próximo bloco
		deslocamento = 0;
		numero_entrada++;
	}
	A->posicao = posicao;
	escreva_entrada(id, &inodo_lida);
	return qn_lido;


}



/**
 * MEU_feof() -
 *
 * @descriptionS
 * 		
 *
 * @param
 * 		
 * @return
 * 		
 */
int meu_feof (MEU_FILE *A)
{

	/*Vai retornar zero  se a corrente for menor q o
	tamanho e retorna 1 se a posição for 
	maior ou igual(chegou no final do arquivo.*/
	struct inodo lida;
	if (A == NULL) return 1;
	uint16_t id = A->id;

	
		
	leia_entrada(id, &lida);
	
	if (lida.tam > A->posicao) return 0;
	return 1;
	
}

/**
 * meu_ftell() - Get current position in file
 *
 * @description
 * 		Returns the current value of the position indicator of the file.
 * 		For binary files, this is the number of bytes from the beginning of the file.
 * 		For text files, the numerical value may not be meaningful but can still be used to
 * 		restore the position to the same position later using fseek (if there are characters
 * 		put back using ungetc still pending of being read, the behavior is undefined).
 *
 * @param
 * 		file: Pointer to a FILE object that identifies the file.
 * @return
 * 		On success, the current value of the position indicator is returned.
 * 		On failure, -1L is returned, and errno is set to a system-specific positive value.
 */

uint16_t meu_ftell(MEU_FILE *A) //RETORNA 0xFFFF se A for Nulo (arquivo estiver fechado)
{
	if (A != NULL)
		return A->posicao; //ftell retorna a posição corrente no arquivo A e 0xFFFF se o arquivo for nulo.
	else{
		return 0xFFFF; 
	}	
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

/** FIM da manipulação de Arquivo **/

/*Escrita e Leitura de Arquivo*/
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
/*FIM Escrita e Leitura de Arquivo*/
