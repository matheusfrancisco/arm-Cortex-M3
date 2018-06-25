// EXEMPLO DE UM Pisca Pisca que usa a Lampada do kit
// para compilar:  
//                    make
// para gravar na placa (usando o bootloader): 
//                   lpc21isp -control -bin main.bin /dev/ttyUSB0 115200 12000

#include "LPC17xx.h"
#include <inttypes.h>
#include "digital.h"
#include "tarefa.h"
#include "DHT11.h"

#define LED0 PIN4_29
#define LED1 PIN3_26


int main() {


	SystemInit();
	uint8_t temperatura;

	
	tarefa.start();

 
 	DHT11.start(PIN4_29);
 	temperatura = DHT11.leTemperatura(); 	
 	
 	
 	
    while (1)
    {
  	
    }
}

