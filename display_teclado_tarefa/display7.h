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




class DISPLAY {
	private:
		volatile uint8_t STATUS;
		volatile uint8_t DIGITOS[4];
		volatile uint8_t k;
		
		

	public:
		void mostra (uint8_t id, uint8_t valor);
		void refresh (void);
		void init (void);
		void mostra (uint16_t valor);
		void mostra (char st[]);
};

extern DISPLAY display;

#endif
