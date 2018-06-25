#include "digital.h"
#include "tarefa.h"
#include "stdlib.h"





TAREFA tarefa;

volatile uint32_t contador;
volatile uint8_t  esta_dento_tratador=0;


void delay_ms (uint32_t t)
{
    if (esta_dento_tratador) return;
	contador=t;
	while (contador>0);
}

extern "C" 
void RIT_IRQHandler(void (*f)(void))
{
	esta_dento_tratador=1;
    LPC_RIT->RICTRL |= 1;
    
    if (contador >=TIC) contador-=TIC/2;
    else contador=0;
     
    tarefa.varre_lista();
	esta_dento_tratador=0;
} 
void TAREFA::desabilita_int(void)
{
    NVIC_DisableIRQ(RIT_IRQn);
}
void TAREFA::habilita_int(void)
{
    NVIC_EnableIRQ(RIT_IRQn);
}
void TAREFA::start ( void )
{
	// configura o tratador para disparar a cada TIC unidades
	// de tempo.
	LPC_SC->PCONP    |= (1<<16);
    LPC_SC->PCLKSEL1 |= ((1<<26) & (1<<27)); 
    LPC_RIT->RICOUNTER = 0;    
    LPC_RIT->RICOMPVAL = 12500*TIC;  
    LPC_RIT->RICTRL |= 2;    
    LPC_RIT->RICTRL |= 8;
    NVIC_SetPriority(RIT_IRQn, 31);
    habilita_int();
    id=0;
    L1.inicio = NULL;
}
void TAREFA::varre_lista ( void )
{
	struct nodo * tmp;
	// varre a lista, decrementa o tempo de cada tarefa
	// se chegou a zero, reinicia o tempo e dispara a execucao
	// da funcao

	tmp = L1.inicio;
	
	while (tmp != NULL)
	{
		if (tmp->contador >=TIC)
		{
			// atualizao tempo
			tmp->contador-=TIC;
		}
		else {
			// dispara a funcao
			tmp->contador = tmp->periodo;
			tmp->funcao();
		}	
		
		tmp = tmp->proximo;
	}	
	
}

int TAREFA::adicionaTarefa ( void (*funcao)(void), int temp )
{
	struct nodo * tmp;
	desabilita_int();
		int valor = id;
		id++;
	//aloca mais um elemento na lista encadeada, para faciliar
	// aloca no inicio da lista,
	tmp = (struct nodo *) malloc ( sizeof(struct nodo)  );
	tmp->funcao = funcao;
	tmp->periodo = temp;
	tmp->contador= temp;
	tmp->id = valor;
	tmp->proximo = L1.inicio;
	L1.inicio = tmp;
	habilita_int();
	  
	return valor;
}

void TAREFA::removeTarefa(uint16_t id)
{
	struct nodo *tmp, *ant=NULL;
	tmp = L1.inicio;
	desabilita_int();
	
	while (tmp != NULL)
	{
		if (tmp->id == id)
		{
			if (L1.inicio != tmp)
			{
				ant->proximo = tmp->proximo;
				free(tmp);
			}
			else {
				L1.inicio = tmp->proximo;
				free(tmp);
			}
			habilita_int();
			return;
		}
		
		ant = tmp;
		tmp = tmp->proximo;
	}
	habilita_int();
}






