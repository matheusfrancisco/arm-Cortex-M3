// EXEMPLO DE UM Pisca Pisca que usa a Lampada do kit
// para compilar:  
//                    make
// para gravar na placa (usando o bootloader): 
//                   lpc21isp -control -bin main.bin /dev/ttyUSB0 115200 12000

#include <stdio.h>
#include "LPC17xx.h"
#include "display7.h"
#include "digital.h"
#include "teclado.h"
#include "tarefa.h"


 Teclado teclado;



int main() {

	char st[10];
	
	SystemInit();

	
	Digital.pinMode(PIN1_29,OUTPUT);
	
	teclado.init(PIN2_00, PIN2_01, PIN2_02, PIN2_03,PIN2_04, PIN2_05, PIN2_06, PIN2_07);
	

	
  tarefa.start();
  display.init();


	while(1){
		teclado.getcaractere(4, st);
		st[4]=0;
		printf("%d",3);
		
	}	
	
	
}
