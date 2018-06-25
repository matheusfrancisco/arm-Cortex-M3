
#include "LPC17xx.h"
#include "i2c_software.h"
#include "digital.h"
#include <stdio.h>
#include "uart.h"
#include <stdint.h>
#include "tarefa.h"

uint8_t PIN_DADOS, PIN_CLOCK;

void i2c_dados_baixo (void)
{
     Digital.pinMode(PIN_DADOS, OUTPUT);
     Digital.digitalWrite(PIN_DADOS,LOW);
}

void i2c_dados_alto (void)
{
     Digital.pinMode(PIN_DADOS, INPUT);
}

void i2c_clk_alto (void)
{
    Digital.pinMode(PIN_CLOCK, INPUT);
}

void i2c_clk_baixo (void)
{
     Digital.pinMode(PIN_CLOCK, OUTPUT);
     Digital.digitalWrite(PIN_CLOCK,LOW);
}

void i2c_configura (uint8_t pino_dados, uint8_t pino_clk)
{
    PIN_DADOS    = pino_dados;
    PIN_CLOCK   = pino_clk;
 tarefa.start();
    // configura como entrada/saida
    i2c_clk_alto();
    i2c_dados_alto();
	delay_ms(1);

}
void i2c_start (void)
{

     i2c_clk_alto();
     delay_us(10);
     i2c_dados_alto();
     delay_us(10);
     i2c_dados_baixo();
 	 delay_us(10);
     i2c_clk_baixo();
 	 delay_us(10);  
}

void i2c_bit (uint8_t valor_bit)
{
    if (valor_bit) i2c_dados_alto();
    else i2c_dados_baixo();
    delay_us(10);
    i2c_clk_alto();
    delay_us(10);
    i2c_clk_baixo();
    i2c_dados_alto();
}

uint8_t i2c_le_bit (void)
{
    uint8_t valor;

    i2c_dados_alto();
    delay_us(10);
    i2c_clk_alto();
    valor=Digital.digitalRead(PIN_DADOS);
    delay_us(10);
    i2c_clk_baixo();
    return valor;
}

uint8_t i2c_write (uint8_t valor_byte)
{
    uint8_t a,x;
    for (x=0;x<8;x++) i2c_bit(1&(valor_byte>>(7-x)));
	a=i2c_le_bit();

	delay_us(10);
    return a;
}

uint8_t i2c_read (void)
{
	uint8_t valor=0,x;
	
	for (x = 0; x < 8; x++) 
		valor = (valor << 1) | i2c_le_bit();
   return valor;
}

uint8_t i2c_read_ACK (uint8_t v)
{
	uint8_t valor=0,x;
	
	for (x = 0; x < 8; x++) valor = (valor << 1) | i2c_le_bit();
	i2c_bit(v);
   return valor;
}

void i2c_stop (void)
{     
      i2c_dados_baixo();
	  delay_us(5);
      i2c_clk_baixo();
      delay_us(5);
	  i2c_clk_alto();
	  delay_us(5);
      i2c_dados_alto();
	  delay_us(5);
}

void i2c_escreve_byte (uint8_t dispositivo, uint16_t end, uint8_t valor)
{
	
	i2c_start();
	    i2c_write (0xA0 | (dispositivo<<1));
	    i2c_write (end>>8);
	    i2c_write (end);
	    i2c_write (valor);
    i2c_stop();

	while (1)
    {
       i2c_start();
       if (i2c_write (0xA0| (dispositivo<<1) )==0) break;
    }
	i2c_stop();
}

uint8_t i2c_le_byte (uint8_t dispositivo, uint16_t end)
{
    uint8_t valor;

    i2c_start();
 	   i2c_write (0xA0 | (dispositivo << 1) );
	   i2c_write (end>>8);
 	   i2c_write (end);
   i2c_start();
    i2c_write (0xA1 | (dispositivo << 1));
    valor = i2c_read();
    i2c_stop();
    return valor;
}

uint16_t i2c_sequencial_read (uint8_t dispositivo, uint16_t end, uint8_t b[], uint16_t qtd)
{
	uint8_t  byte_controle;
	uint16_t x;

	//printf ( "i2c_sequencial_read (end=%d, qtd=%d)\n", end , qtd );

	// comeca com um write bobo para salvar o endereco dentro do dispositivo
	i2c_start ( );
		byte_controle = (0xA0 | (dispositivo << 1) );
		i2c_write ( byte_controle );
		i2c_write ( end >> 8 );
		i2c_write ( end );

	// Sequencial current read inicia neste ponto
	i2c_start ( );
		byte_controle = (0xA1 | (dispositivo << 1));      // muda para read
		i2c_write ( byte_controle );
		for ( x = 0; x < qtd-1; x++ )  b[x] = i2c_read_ACK(0);
		b[x] = i2c_read_ACK(1);
	i2c_stop();


	return qtd;
}

// ******************************** Le apenas um byte ************************************************
uint16_t i2c_sequencial_write (uint8_t dispositivo, uint16_t end, uint8_t b[], uint16_t qtd)
{
	uint8_t x;
	

	//printf ( "i2c_sequencial_write (end=%d, qtd=%d)\n", end , qtd );
	
   	i2c_start();
		i2c_write (0xA0 | (dispositivo << 1) );
		i2c_write ( end >> 8 );
		i2c_write ( end );
		for ( x = 0; x < qtd; x++ ) i2c_write ( b[x] );
	i2c_stop();

	// verifica se terminou a gravacao
	while(1)
	{
		i2c_start();
		if (i2c_write (0xA0 | (dispositivo << 1))==0) break;
	}
	i2c_stop();
	return qtd;
}
// ****************************************************************************************************

