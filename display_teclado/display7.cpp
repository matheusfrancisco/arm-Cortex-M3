#include <string.h>
#include "display7.h"
#include "delay.h"

void DISPLAY::init(void)
{
	Digital.pinMode(SEG_A, OUTPUT);
	Digital.pinMode(SEG_B, OUTPUT);
	Digital.pinMode(SEG_C, OUTPUT);
	Digital.pinMode(SEG_D, OUTPUT);
	Digital.pinMode(SEG_E, OUTPUT);
	Digital.pinMode(SEG_F, OUTPUT);
	Digital.pinMode(SEG_G, OUTPUT);
	
	Digital.pinMode(DISPLAY_0, OUTPUT);
	Digital.pinMode(DISPLAY_1, OUTPUT);
	Digital.pinMode(DISPLAY_2, OUTPUT);
	Digital.pinMode(DISPLAY_3, OUTPUT);
	
	Digital.digitalWrite(SEG_A, HIGH);
	Digital.digitalWrite(SEG_B, HIGH);
	Digital.digitalWrite(SEG_C, LOW);
	Digital.digitalWrite(SEG_D, LOW);
	Digital.digitalWrite(SEG_E, LOW);
	Digital.digitalWrite(SEG_F, LOW);
	Digital.digitalWrite(SEG_G, LOW);
	
	
	
	Digital.digitalWrite(DISPLAY_0, HIGH);
	Digital.digitalWrite(DISPLAY_1, HIGH);
	Digital.digitalWrite(DISPLAY_2, LOW);
	Digital.digitalWrite(DISPLAY_3, LOW);
	
}
void DISPLAY::mostra (char st[])
{
	uint8_t tmp[4]={0,0,0,0};
	
	for (int x=0;x < strlen(st);x++)
	{
		if ((st[x] >= '0') && (st[x]<='9')) tmp[3-x] = st[x]-48;
		else tmp[3-x] = st[x]-55;
	}
	while (1)
	{
		for (int k=0; k <4; k++)
			mostra(k, tmp[k]);
	}
}

void DISPLAY::mostra (uint8_t id, uint8_t valor)
{
	uint8_t valor_bits = vet_simb[valor];
	
	for (int x=0;x<4;x++) Digital.digitalWrite(habilita[x],LOW);
	Digital.digitalWrite(habilita[id],HIGH);
	
	for (int x=0;x<7;x++)
		if ((valor_bits >>x) &1)  Digital.digitalWrite(pinos[x],HIGH);
		else  Digital.digitalWrite(pinos[x],LOW);
		
	delay();
}

void DISPLAY::mostra (uint16_t valor)
{
	uint8_t v[4];
	uint16_t tmp;
	
	tmp = valor;
	for (int x=0;x<4;x++)
	{
		v[3-x] = tmp % 10;
		tmp = tmp/10;
	}
	
	while (1)
	{
		for (int x=0;x<4;x++)
		{
			mostra(x, v[x]);	
		}
	}
		
}
DISPLAY display;
