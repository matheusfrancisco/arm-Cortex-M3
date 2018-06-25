#include "teclado.h"
#include "delay.h"


void Teclado::init ( uint8_t C0, uint8_t C1, uint8_t C2, uint8_t C3, uint8_t L0, uint8_t L1, uint8_t L2, uint8_t L3 )
{
	pin_C0 = C0; pin_C1 = C1; pin_C2 = C2; pin_C3 = C3;
	pin_L0 = L0; pin_L1 = L1; pin_L2 = L2; pin_L3 = L3;
   
	Digital.pinMode(pin_C0, OUTPUT);
	Digital.pinMode(pin_C1, OUTPUT);
	Digital.pinMode(pin_C2, OUTPUT);
	Digital.pinMode(pin_C3, OUTPUT);
	
	Digital.pinMode(pin_L0, INPUT);
	Digital.pinMode(pin_L1, INPUT);
	Digital.pinMode(pin_L2, INPUT);
	Digital.pinMode(pin_L3, INPUT);

	modoAtual = BLOQ;
}
uint8_t Teclado::leiaLinha (uint8_t n)
{
	uint8_t colunas[]={pin_C0,pin_C1,pin_C2,pin_C3};
	uint8_t linhas[]={pin_L0, pin_L1, pin_L2, pin_L3};
	
	for (int c=0;c<4;c++) Digital.digitalWrite(colunas[c],HIGH);
	
	int c;
	for (c=0;c<4;c++)
	{
		Digital.digitalWrite(colunas[c], LOW);	
		if (Digital.digitalRead(linhas[n])==LOW)
			break;		
		Digital.digitalWrite(colunas[c], HIGH);	
		
	}
	return c;
}
uint8_t Teclado::leiaBotao (void)
{
	char M[4][4]={ {'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'E','0','F','D'} } ;
	
	int C;
	
	while (1)
	{
		for (int L=0;L<4;L++)
		{
			C = leiaLinha(L);
			if (C <4)
			{
				while (leiaLinha(L) != 4);
				return M[L][C];
			}
			//if (C!= 4) return M[L][C];
		}
	}	
}
uint8_t Teclado::getchar (uint8_t qtd, char *st)
{
	if (modoAtual==BLOQ)
	{
		for (int q = 0; q< qtd; q++)
			st[q] = leiaBotao();
	}	
	
	return 0;	
}
