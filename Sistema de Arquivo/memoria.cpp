#include <inttypes.h>
#include "memoria.h"
#include "i2c_software.h"

#include <stdio.h>

#define MIN(a,b) (((a)<(b))?(a):(b))

uint8_t DISPOSITIVO;
uint8_t TAM_PAGINA;

#ifdef _LINUX_

#include <stdio.h>

FILE *a;
void memoria_i2c::init (void )
{

		a = fopen ("PARTICAO.bin","r+");
		if (a == NULL)
		{
					a = fopen ("PARTICAO.bin","w+");
					
					
		}
	
}

void memoria_i2c::read(uint16_t end, uint16_t tam, uint8_t *b)
{
		fseek(a, end, SEEK_SET);
		fread(b, 1, tam, a);
}
void memoria_i2c::write(uint16_t end, uint16_t tam, uint8_t *b)
{

			fseek(a, end, SEEK_SET);
			fwrite(b, 1, tam, a);
}

#else
#include "digital.h"


#include "i2c_software.h"
void memoria_i2c::read(uint16_t end, uint16_t tam, uint8_t *b)
{
	uint16_t R, qtd, contador=0;

	while (tam>0)
	{
		R = (end / TAM_PAGINA) * TAM_PAGINA +TAM_PAGINA;
		qtd = i2c_sequencial_read (DISPOSITIVO, end, b+contador, MIN(R - end, tam));
		contador+=qtd;
		end+=qtd;  tam-=qtd;
	}
}

void memoria_i2c::init (void)
{

	DISPOSITIVO = 0;
	TAM_PAGINA  = 32;
	

	i2c_configura (PIN0_27, PIN0_28);
	//i2c_configura (PIN_2_05, PIN_1_31);
	i2c_start();
	i2c_stop();

}





void memoria_i2c::write(uint16_t end, uint16_t tam, uint8_t *b)
{

	uint16_t R, qtd,  contador=0;

	while (tam>0)
	{
		R = (end / TAM_PAGINA) * TAM_PAGINA +TAM_PAGINA;
		qtd = i2c_sequencial_write (DISPOSITIVO, end, b+contador, MIN(R - end, tam));
		end+=qtd;  tam-=qtd;
		contador+=qtd;
	}
}



#endif

memoria_i2c memoria;

