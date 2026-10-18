#include "stm32f4xx.h"

void Set_Interrupt(IRQn Int_Num, uint8_t Prior);

void Clr_Interrupt(IRQn Int_Num);

void Clr_Interrupt_PD(IRQn Int_Num);
