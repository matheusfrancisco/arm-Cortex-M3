// EXEMPLO DE UM Pisca Pisca que usa a Lampada do kit
// para compilar:  
//                    make
// para gravar na placa (usando o bootloader): 
//                   lpc21isp -control -bin main.bin /dev/ttyUSB0 115200 12000



#include "LPC17xx.h"
#include "display7.h"
#include "delay.h"



int main() {
  
  	  	
	SystemInit();

	display.init();
	display.mostra(9876);
	
	
  
  while (1)
  {
	 
     
  }

	
	
	
}
