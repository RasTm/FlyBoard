#include "stm32f4xx.h"

void dma_start();
void dma_stop();

typedef struct{
	DMA_TypeDef* dma_periph;

}DMA;
