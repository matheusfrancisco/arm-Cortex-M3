#ifndef __TAREFA__
#define __TAREFA__
#include "LPC17xx.h"
#include <inttypes.h>
#include "digital.h"
#define TIC 100 // 100ms

void delay_ms (uint32_t t);
void delay_us (uint32_t t);

struct nodo {
	void (*funcao)(void);
	uint32_t periodo;
	uint32_t contador;
	uint16_t id;
	struct nodo * proximo;
};

typedef struct {
	struct nodo * inicio;

}Tipo_Lista;





class TAREFA {

	private:
		Tipo_Lista L1;
		uint16_t id;
		void desabilita_int(void);
		void habilita_int(void);
	public:
		void start (void);
		void varre_lista (void);
		int adicionaTarefa ( void (*funcao)(void), int temp );
		void removeTarefa( uint16_t id);

};

extern TAREFA tarefa;

#endif
