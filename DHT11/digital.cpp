#include "digital.h"
#include "LPC17xx.h"

void    DIGITAL::digitalWrite ( uint8_t pino, uint8_t valor)
{
	uint8_t porta = pino / 32;
	uint8_t bit   = pino % 32;

		switch (porta)
		{
			case 0:
				if (valor==HIGH) LPC_GPIO0->FIOSET |= (1 << bit);
				else LPC_GPIO0->FIOCLR |= (1 << bit);
				break;
			case 1:
				if (valor==HIGH) LPC_GPIO1->FIOSET |= (1 << bit);
				else LPC_GPIO1->FIOCLR |= (1 << bit);
				break;
			case 2:
				if (valor==HIGH) LPC_GPIO2->FIOSET |= (1 << bit);
				else LPC_GPIO2->FIOCLR |= (1 << bit);
				break;
			case 3:
				if (valor==HIGH) LPC_GPIO3->FIOSET |= (1 << bit);
				else LPC_GPIO3->FIOCLR |= (1 << bit);
				break;
			case 4:
				if (valor==HIGH) LPC_GPIO4->FIOSET |= (1 << bit);
				else LPC_GPIO4->FIOCLR |= (1 << bit);
			
		}

}


void    DIGITAL::pinMode ( uint8_t pino, uint8_t tipo )
{
	uint8_t porta = pino / 32;
	uint8_t bit   = pino % 32;
	
	if (tipo == OUTPUT)
	{
		switch (porta)
		{
			case 0:
				LPC_GPIO0->FIODIR |= (1 << bit);
				break;
			case 1:
				LPC_GPIO1->FIODIR |= (1 << bit);
				break;
			case 2:
				LPC_GPIO2->FIODIR |= (1 << bit);
				break;
			case 3:
				LPC_GPIO3->FIODIR |= (1 << bit);
				break;
			case 4:
				LPC_GPIO4->FIODIR |= (1 << bit);
			
		}
	}
	else
	{
		switch (porta)
		{
			case 0:
				LPC_GPIO0->FIODIR &= ~(1 << bit);
				break;
			case 1:
				LPC_GPIO1->FIODIR &= ~(1 << bit);
				break;
			case 2:
				LPC_GPIO2->FIODIR &= ~(1 << bit);
				break;
			case 3:
				LPC_GPIO3->FIODIR &= ~(1 << bit);
				break;
			case 4:
				LPC_GPIO4->FIODIR &= ~(1 << bit);
			
		}
	}
	


}

uint8_t DIGITAL::digitalRead ( uint8_t pino )
{
  uint32_t lido;
	uint8_t porta = pino / 32;
  uint8_t bit   = pino % 32;

  switch (porta)
    {
      case 0:
        lido = LPC_GPIO0->FIOPIN;
        break;
      case 1:
        lido = LPC_GPIO1->FIOPIN;
        break;
      case 2:
        lido = LPC_GPIO2->FIOPIN;
        break;
      case 3:
        lido = LPC_GPIO3->FIOPIN;
        break;
      case 4:
        lido = LPC_GPIO4->FIOPIN;
      
    }
    lido = ( lido >> bit) & 1;
    return lido;
}

DIGITAL Digital;


