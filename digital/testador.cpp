// EXEMPLO DE UM Pisca Pisca que usa a Lampada do kit
// para compilar:  
//                    make
// para gravar na placa (usando o bootloader): 
//                   lpc21isp -control -bin main.bin /dev/ttyUSB0 115200 12000



#include "LPC17xx.h"
#include "digital.h"
#include "serial.h"

#define Periodo 1000000
int cicloAlto = Periodo/2;

long delay_longo()
{
  long y=0;
  for (long x=0;x<1000;x++)
  {
  	for (long z=0;z<100;z++) y = y+x+z;
  }
  return y;
}

long delay(long valor)
{
  long y=0;
  for (long x=0;x<valor;x++)
  {
     y = y+x;
  }
  return y;
}

int main() {
  
	SystemInit();

	pinMode(PIN1_29, OUTPUT);
	pinMode(PIN1_23, INPUT);

	  
  while (1)
  {
	 digitalWrite(PIN1_29,HIGH);
	 
   delay(cicloAlto);
     digitalWrite(PIN1_29,LOW);
     delay(Periodo-cicloAlto);
     
     if (digitalRead(PIN1_23)==HIGH)
     {
      if ((cicloAlto-100000) >0)   	cicloAlto = cicloAlto-100000;
     	while (digitalRead(PIN1_23)==HIGH)
     	{
     	}
     	delay_longo();
     }
     
  }

	
	
	
}
