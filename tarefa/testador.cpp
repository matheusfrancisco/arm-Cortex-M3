// EXEMPLO DE UM Pisca Pisca que usa a Lampada do kit
// para compilar:  
//                    make
// para gravar na placa (usando o bootloader): 
//                   lpc21isp -control -bin main.bin /dev/ttyUSB0 115200 12000

#include "LPC17xx.h"
#include <inttypes.h>
#include "digital.h"
#include "tarefa.h"

#define LED0 PIN4_29
#define LED1 PIN3_26
uint8_t statusLED0=HIGH, statusLED1=HIGH;


void func1 (void)
{
	Digital.digitalWrite(LED0, statusLED0);
	statusLED0 = !statusLED0;
}
void func2 (void)
{
	Digital.digitalWrite(LED1, statusLED1);
	statusLED1 = !statusLED1;
}

int main() {

	uint16_t id1;
	SystemInit();

	Digital.pinMode(LED0, OUTPUT);
	Digital.pinMode(LED1, OUTPUT);
	tarefa.start();
	id1=tarefa.adicionaTarefa ( func1, 100 );
	tarefa.adicionaTarefa ( func2, 300 );
 
 
 	delay_ms(15000);
 	tarefa.removeTarefa(id1);
    while (1)
    {
  	
    }
}

