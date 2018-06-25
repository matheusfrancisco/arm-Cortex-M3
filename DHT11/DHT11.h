#ifndef __DHT11__
#define __DHT11__
#include <inttypes.h>

class __DHT__ {
	private:
		uint8_t meuPino;

	public:
		void    start (uint8_t PINO);
		uint8_t leTemperatura(void);
	
};

extern __DHT__ DHT11;

#endif

