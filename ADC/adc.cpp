#include "LPC17xx.h"
#include "adc.h"


#define ADC_POWERON (1 << 12) 
#define PCLK_ADC 24
#define PCLK_ADC_MASK (3 << 24)
// AD0.0 - P0.23, PINSEL1 [15:14] = 01
#define SELECT_ADC0 (0x1<<14)
// ADOCR constants
#define START_ADC (1<<24)
#define OPERATIONAL_ADC (1 << 21)
#define SEL_AD0 (1 <<0)
#define ADC_DONE_BIT	(1 << 31)


int le_adc (void)
{

	int adval;
	
	// Turn on power to ADC block 
	LPC_SC->PCONP |=  ADC_POWERON;

	// Turn on ADC peripheral clock
	LPC_SC->PCLKSEL0 = LPC_SC->PCLKSEL0 & ~(PCLK_ADC_MASK);
	LPC_SC->PCLKSEL0 |=  (3 << PCLK_ADC);
		
	// Set P0.23 to AD0.0 in PINSEL1
	LPC_PINCON->PINSEL1	|= SELECT_ADC0; 
	
	// Enter an infinite loop, just checking ADC pot and incrementing a counter
	while(1) {
		// Start A/D conversion for on AD0.0
		LPC_ADC->ADCR = START_ADC | OPERATIONAL_ADC | SEL_AD0 ;

		do {
			adval = LPC_ADC->ADGDR;              // Read A/D Data Register
		} while ((adval & ADC_DONE_BIT) == 0);   // Wait for end of A/D Conversion
		
		LPC_ADC->ADGDR &= ~(START_ADC | OPERATIONAL_ADC | SEL_AD0 );   // Stop A/D Conversion

		 // Extract AD0.0 value - 12 bit result in bits [15:4]
		adval = (adval >> 4) & 0x0FFF ;            
		return adval;
	}
	return 0;
}
