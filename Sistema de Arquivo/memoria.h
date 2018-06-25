
#ifndef _MEMORIA_I2C_
#define _MEMORIA_I2C_

#include <stdint.h>
#include <stdint.h>




class memoria_i2c {

	private:
		uint8_t dispositivo;
	public:
		void init (void );
		void write(uint16_t end, uint16_t tam, uint8_t *b);
		void read(uint16_t end, uint16_t tam, uint8_t *b);

};

extern memoria_i2c memoria;

#endif
