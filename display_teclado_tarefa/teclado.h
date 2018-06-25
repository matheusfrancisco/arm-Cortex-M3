#ifndef __TECLADO__
#define __TECLADO__
#include "inttypes.h"
#include "digital.h"

#define BLOQ 1
#define NBLOQ 0


class Teclado {

	private:
		uint8_t pin_C0, pin_C1, pin_C2, pin_C3;
		uint8_t pin_L0, pin_L1, pin_L2, pin_L3;
		uint8_t modoAtual;
		uint8_t leiaLinha(uint8_t n);
		uint8_t leiaBotao(void);
		
	public:
		void init ( uint8_t C0, uint8_t C1, uint8_t C2, uint8_t C3, uint8_t L0, uint8_t L1, uint8_t L2, uint8_t L3 );
		void modo (uint8_t modo_operacao);
		uint8_t getcaractere (uint8_t qtd, char *st);
};


#endif



