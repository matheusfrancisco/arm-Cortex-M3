#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "SA.h"

int main (void)
{

	formata();
	
	MEU_FILE *A, *B, *C;
	
	A = meu_fopen ("teste","w");
	if (A==NULL)
	{
		printf("Nao consegue criar o arquivo\n");
	}
	
	for (int x=0;x<32;x++) 	meu_fputc(65+x,A);
	for (int x=0;x<32;x++) 	meu_fputc('A',A);
	for (int x=0;x<32;x++) 	meu_fputc('B',A);
	meu_fseek(A, 33);
	meu_fputc('X', A);
	
		B = meu_fopen ("UFSC","w");
		meu_fputc('U',B);
		meu_fputc('F',B);
		meu_fputc('S',B);
		meu_fputc('C',B);
			meu_fseek(A, 33);
			printf("Lido = %c\n", meu_fgetc(A));
	meu_fseek(A, 0);


	while (!meu_feof(A))
	{
		printf("%c",meu_fgetc(A));
	}
	printf("\n\n");


	meu_fclose(A);
	meu_fclose(B);

	//uint16_t meu_fwrite(MEU_FILE *A, void * buffer, uint16_t tamanho, uint16_t count)
	C = meu_fopen("doido", "w");
	
	if(C==NULL) printf("Erro");

	char txt[1000];

	strcpy(txt,"Cpp,ppp como vai você?");


	meu_fwrite(C, (uint8_t*) txt, sizeof(char), strlen(txt));

	meu_fclose(C);

	C = meu_fopen("huehue", "w");
	meu_fwrite(C, (uint8_t*) txt, sizeof(char), strlen(txt));

	meu_fseek(C,0);


	//MEU_FILE *A, uint8_t * buffer, uint16_t size, uint16_t count

	uint8_t * x; 
	meu_fread(C, (uint8_t*) x, sizeof(char), 1);

	while (!meu_feof(C))
	{
		printf("%c",meu_fgetc(C));
	}

	meu_fclose(C);
	
}
