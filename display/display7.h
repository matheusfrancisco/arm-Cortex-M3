#ifndef __DISPLAY_7_
#define __DISPLAY_7_

#include "digital.h"
#include "inttypes.h"

#define SEG_A PIN0_04
#define SEG_B PIN0_05
#define SEG_C PIN0_06
#define SEG_D PIN0_07
#define SEG_E PIN0_08
#define SEG_F PIN0_09
#define SEG_G PIN0_10


#define DISPLAY_0 (PINO(3,25))
#define DISPLAY_1 (PINO(3,26))
#define DISPLAY_2 (PINO(4,28))
#define DISPLAY_3 (PINO(4,29))


//uint8_t vet_simb[]= {0x3f,0,0,0x4f,0,0,0,0};

//uint8_t pinos[]= {SEG_A, SEGB, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G};

class DISPLAY {
	private:
		uint8_t vet_simb[20]= {0x3f,0x6,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
		uint8_t pinos[7]= {SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G};
		uint8_t habilita[4]={DISPLAY_0,DISPLAY_1, DISPLAY_2, DISPLAY_3};
	public:
	
		void init (void);
		void mostra (uint8_t id, uint8_t valor);
		void mostra (uint16_t valor);
};

extern DISPLAY display;

#endif
