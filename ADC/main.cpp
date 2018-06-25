//                   lpc21isp -control -bin main.bin /dev/ttyUSB0 115200 12000

#include "LPC17xx.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "adc.h"



int main(void)
{
	uint32_t adc;



	SystemInit();
	

	while (1)
	{
		adc = le_adc ();
		
	
		printf("%ld\n",adc);
		
	}
		


			
	return 0;
}

