#include "Nvic.hpp"

/**
  * @brief  This function allows you to set interrupt.
  * @param  Int_Num: This variable holds your interrupt number.(Look for stm32f4xx.h file)
  * @param  Prior  : This variable holds your interrupt priority.
  * @retval
  */
void Set_Interrupt(IRQn Int_Num, uint8_t Prior){

	NVIC-> ISER[Int_Num/32] |= (1<<(Int_Num%32));

	NVIC-> IP[Int_Num] = Prior;
}

/**
  * @brief  This function allows you to set interrupt.
  * @param  Int_Num: This variable holds your interrupt number.(Look for stm32f4xx.h file)
  * @retval
  */
void Clr_Interrupt(IRQn Int_Num){

	NVIC-> ICER[Int_Num/32] |= (1<<(Int_Num%32));
}

/**
  * @brief  This function allows you to set interrupt.
  * @param  Int_Num: This variable holds your interrupt number.(Look for stm32f4xx.h file)
  * @retval
  */
void Clr_Interrupt_PD(IRQn Int_Num){

	NVIC-> ICPR[Int_Num/32] |= (1<<(Int_Num%32));
}
