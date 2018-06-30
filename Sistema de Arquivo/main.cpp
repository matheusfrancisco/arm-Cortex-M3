#include "LPC17xx.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "uart.h"
#include "SA.h"


int main (void)
{


	SystemInit();
	  
	UART0_Init(9600);


	formata();
	char buffer[70];
	char buffer2[70];
	MEU_FILE *A,*C;
	//uint16_t meu_fread(MEU_FILE *A, uint8_t * buffer, uint16_t size, uint16_t count) 
	//uint16_t meu_fwrite(MEU_FILE *A, uint8_t * buffer, uint16_t size, uint16_t count) {

	A = meu_fopen ("UFSC", "w");
	strcpy(buffer, "abcdefghijklmnopqrstuvwxyz123456789abcdefghijklmnopqrstuvwxyz123456789abcdefghijklmnopqrstuvwxyz123456789");
	meu_fwrite(A,  ( uint8_t*) buffer ,sizeof(char), strlen(buffer));
	meu_fclose(A);


	A = meu_fopen("UFSC","r+");
	meu_fseek(A,0);
	printf("%d\n", meu_ftell(A));
	meu_fread( A,(uint8_t *) buffer2, sizeof(char), strlen(buffer));
	printf("Lido = %s\n", buffer2);
	meu_fclose(A);

	while(1){}
}
