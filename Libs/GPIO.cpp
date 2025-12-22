#include "GPIO.hpp"

/**
  * @brief  This function allows you to configure Gpio pins.
  * @param  Gpio_Port: This pointer holds which Gpio port you want to configure.
  * @param  Pin  : This variable contains which port pin you want to configure.
  * @param  Mode : This variable contains which mode you want to select. (Look GPIO.hpp for more info)
  * @param  OType: This variable select your Output Type of desired pin. (Look GPIO.hpp for more info)
  * @param  Speed: This variable select your pin speed. (Look GPIO.hpp for more info)
  * @param  Pupdr: This variable activate Pull Up or Pull Down resistors of desired pin.
  * @param  AFR  : This variable select Alternate Function for desired pin. (Look datasheet for more info)
  * @retval -
  */
void gpio_config(GPIO_TypeDef *Gpio_Port, uint8_t Pin, uint8_t Mode, uint8_t OType, uint8_t Speed, uint8_t Pupdr, Alternate_ AFR){

//	uint8_t Gpio_field = 0;

//	if(Mode == INPUT) 	     Gpio_field = 0;
//	else if(Mode == OUTPUT)  Gpio_field = 1;
//	else if(Mode == ALTER)   Gpio_field = 2;
//	else if(Mode == ANALOG)  Gpio_field = 3;
	Gpio_Port-> MODER  |=   (Mode<<(Pin*2));

//	if(OType == PUSH)		 Gpio_field = 0;
//	else if(OType == DRAIN)  Gpio_field = 1;
	Gpio_Port-> OTYPER |=   (OType<<Pin);

//	if(Speed == LOW) 		Gpio_field = 0;
//	else if(Speed == MED)	Gpio_field = 1;
//	else if(Speed == HIGH)	Gpio_field = 2;
//	else if(Speed == V_HIGH)Gpio_field = 3;
	Gpio_Port-> OSPEEDR|=  (Speed<<(Pin*2));

//	if(Pupdr == NOT)		Gpio_field = 0;
//	else if(Pupdr == UP)	Gpio_field = 1;
//	else if(Pupdr == DOWN)  Gpio_field = 2;
	Gpio_Port-> PUPDR  |=  (Pupdr<<(Pin*2));

	if(Pin>7){Gpio_Port-> AFR[1] |= (AFR<<((Pin-8)*4));}
	else{Gpio_Port-> AFR[0] |= (AFR<<(Pin*4));}
}

/*
 * @brief This function reset all configuration for desired Gpio pin
 * @param  Gpio_Port: This pointer holds which Gpio port you want to configure.
 * @param  Pin      : This variable contains which port pin you want to configure.
 * @retval -
 */
void reset_gpio_config(GPIO_TypeDef *Gpio_Port, uint8_t Pin){
	Gpio_Port-> MODER   &= ~(3<<Pin*2);
	Gpio_Port-> OTYPER  &= ~(1<<Pin);
	Gpio_Port-> OSPEEDR &= ~(3<<Pin*2);
	Gpio_Port-> PUPDR   &= ~(3<<Pin*2);
	Gpio_Port-> BSRRH   |= (1<<Pin);

	if(Pin>7){
		Gpio_Port-> AFR[1] &= ~(0xF<<((Pin-8)*4));
		Gpio_Port-> AFR[1] |= (SyS<<((Pin-8)*4));
	}
	else{
		Gpio_Port-> AFR[0] &= ~(0xF<<(Pin*4));
		Gpio_Port-> AFR[0] |= (SyS<<(Pin*4));
	}
}

/**
  * @brief  This function allows you to write Gpio pins.
  * @param  Gpio_Port: This pointer holds which Gpio port you want to configure.
  * @param  Pin      : This variable contains which port pin you want to configure.
  * @param  set_reset: This variable set desired pin to HIGH or LOW logic state.
  * @retval -
  */
void write_gpio_io(GPIO_TypeDef *Gpio_Port, uint8_t Pin, uint8_t set_reset){

	if(set_reset > 0){
		Gpio_Port-> BSRRL |= (1<<Pin);   //Set
	}
	else{
		Gpio_Port-> BSRRH |= (1<<Pin);   //Reset
	}
}

/*
 * @brief This function allows you to read Gpio pins.
 * @param  Gpio_Port: This pointer holds which Gpio port you want to configure.
 * @param  Pin      : This variable contains which port pin you want to configure.
 * @retval Gpio pin state.
 */
uint8_t read_gpio_io(GPIO_TypeDef *Gpio_Port, uint8_t Pin){
	return (Gpio_Port-> IDR &= (1<<Pin));
}

/**
  * @brief  This function allows you to configure External Interrupt pins.
  *
  * @param  Exti     : This variable contains which External Interrupt line you want to configure.(Look datasheet for more info)
  * @param  GPIO_Port: This variable contains which port you want to configure.
  * @param  r_f      : This variable decides which direction triggers External Interrupt.
  * @retval -
  */
void Set_Ext_Interrupt(uint8_t Exti, uint8_t GPIO_Port, uint8_t r_f){

	SYSCFG-> EXTICR[Exti/4] |= (GPIO_Port<<((Exti%4)*4));

	EXTI-> IMR |= (1<<Exti);

	if(r_f == RISING){
		EXTI-> RTSR |= (1<<Exti);
	}
	else if(r_f == FALLING){
		EXTI-> FTSR |= (1<<Exti);
	}
	else if(r_f == BOTH){
		EXTI-> RTSR |= (1<<Exti);
		EXTI-> FTSR |= (1<<Exti);
	}
}
/**
  * @brief  This function allows you to clear External Interrupt Pending Bits.
  * @param  Exti: This variable contains which External Interrupt Pending Bit you want to clear.(Look datasheet for more info)
  * @retval -
  */
void Clr_Ext_Interrupt_PD(uint8_t Exti){

	EXTI-> PR |= (1<<Exti);
}
