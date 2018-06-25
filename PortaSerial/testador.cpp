// EXEMPLO DE UM Pisca Pisca que usa a Lampada do kit
// para compilar:  
//                    make
// para gravar na placa (usando o bootloader): 
//                   lpc21isp -control -bin main.bin /dev/ttyUSB0 115200 12000

#include "LPC17xx.h"
#include <stdio.h>
#include "uart.h"

int main()
{
  
	SystemInit();

		  
    UART0_Init(9600);
    float f=4.56;
    int x=10;
    char nome[10];
	while (1)
	{
		gets(nome);
		printf("Lido=%s\n",nome);

	}
	
}

