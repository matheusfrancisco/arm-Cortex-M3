
#include "delay.h"

int delay (void)
{
	int y=0;
	for (int x=0;x<200000;x++)
	{
		y = y+x;
	}
	
	return y;
}
