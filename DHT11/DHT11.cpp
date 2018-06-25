#include "DHT11.h"
#include "digital.h"
#include "tarefa.h"



void __DHT__::start (uint8_t PINO)
{
    //pinMode(8, OUTPUT);
  	//digitalWrite(8,LOW);
  	meuPino = PINO;
  	Digital.pinMode(PINO, OUTPUT);
  	Digital.digitalWrite(PINO, LOW);
  	delay_ms(20);
}

uint8_t __DHT__::leTemperatura (void)
{
	uint8_t resposta, v;
	
	Digital.pinMode(meuPino, INPUT);
  	delay_us(120);
  	while (Digital.digitalRead(meuPino)==HIGH);

	while (Digital.digitalRead(meuPino)==LOW);
	resposta = 0;
    for (int x=0;x<40;x++)
    {
      delay_us(35);
      v = Digital.digitalRead(meuPino);
      if ((x >=16) && (x<=23) )
      {
      	resposta = resposta  | (v << (23-x));
      }
      
      if (v==HIGH) while (Digital.digitalRead(meuPino)==HIGH);
      while (Digital.digitalRead(meuPino)==LOW);
    }
    
    return resposta;
	
}


 __DHT__ DHT11;
