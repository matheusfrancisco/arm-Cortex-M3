// EXEMPLO DE UM Pisca Pisca que usa a Lampada do kit
// para compilar:  
//                    make
// para gravar na placa (usando o bootloader): 
//                   lpc21isp -control -bin main.bin /dev/ttyUSB0 115200 12000



#include "LPC17xx.h"
#include "display7.h"
#include "delay.h"
#include "digital.h"
#include "teclado.h"

  	Teclado teclado;

int main() {

	char st[10];
	
	SystemInit();
	display.init();
	
	teclado.init(PIN2_00, PIN2_01, PIN2_02, PIN2_03,PIN2_04, PIN2_05, PIN2_06, PIN2_07);
	

	teclado.getchar(4, st);
	st[4]=0;
	
	
				display.mostra(st);
  while (1)
  {
	 
     
  }

	
	
	
}
