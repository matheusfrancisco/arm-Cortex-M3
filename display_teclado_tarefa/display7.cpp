#include <stdio.h>
#include <string.h>
#include "display7.h"
#include "tarefa.h"

extern "C" 
int _write (int fd, const void *buf, size_t count)
{
	Digital.digitalWrite(PIN1_29,HIGH);

	char *vet = (char *) buf;
	display.mostra(88);
	//for (x=0;x<count;x++) display.mostra(x,vet[x]);
	return count;
}

void ATUALIZADISPLAY (void)
{
	display.refresh();
}

void DISPLAY::refresh(void)
{
	static int k=0;
	mostra(k, DIGITOS[k]);
	k = (k + 1) % 4;
}
void DISPLAY::init(void)
{
	k=0;
	STATUS=0;
	DIGITOS[0]=0;
		DIGITOS[1]=0;
			DIGITOS[2]=0;
				DIGITOS[3]=0;
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
	
	Digital.digitalWrite(SEG_A, LOW);
	Digital.digitalWrite(SEG_B, LOW);
	Digital.digitalWrite(SEG_C, LOW);
	Digital.digitalWrite(SEG_D, LOW);
	Digital.digitalWrite(SEG_E, LOW);
	Digital.digitalWrite(SEG_F, LOW);
	Digital.digitalWrite(SEG_G, LOW);
	
	
	
	Digital.digitalWrite(DISPLAY_0, HIGH);
	Digital.digitalWrite(DISPLAY_1, HIGH);
	Digital.digitalWrite(DISPLAY_2, HIGH);
	Digital.digitalWrite(DISPLAY_3, HIGH);
	
	tarefa.adicionaTarefa( ATUALIZADISPLAY, 1);
	
}

void DISPLAY::mostra (char st[])
{
	DIGITOS[0]=0;
	DIGITOS[1]=0;
	DIGITOS[2]=0;
	DIGITOS[3]=0;



	for (uint16_t x=0;x < strlen(st);x++)
	{
		if ((st[x] >= '0') && (st[x]<='9')) DIGITOS[4-strlen(st)+x] = st[x]-48;
		else DIGITOS[4-strlen(st)+x] = st[x]-55;
	}
	
}

void DISPLAY::mostra (uint8_t id, uint8_t valor)
{
	
volatile uint8_t vet_simb[20]= {0x3f,0x6,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f, 0x77,0x7c, 0x39,0x5e,0x79,0x71};
volatile uint8_t pinos[7]= {SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G};
		volatile uint8_t habilita[4]={DISPLAY_0,DISPLAY_1, DISPLAY_2, DISPLAY_3};

	volatile uint8_t valor_bits = vet_simb[valor];
	
	for (volatile int x=0;x<4;x++) Digital.digitalWrite(habilita[x],LOW);
	
	Digital.digitalWrite(habilita[id],HIGH);
	
	for (int x=0;x<7;x++) {
		if ((valor_bits >>x) &1)  Digital.digitalWrite(pinos[x],HIGH);
		else  Digital.digitalWrite(pinos[x],LOW);
		
	}
}

void DISPLAY::mostra (uint16_t valor)
{
	
	uint16_t tmp;
	
	tmp = valor;
	for (int x=0;x<4;x++)
	{
		DIGITOS[3-x] = tmp % 10;
		tmp = tmp/10;
	}
	
	
		
}
DISPLAY display;
