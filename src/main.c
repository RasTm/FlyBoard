/*********************************************************
 *	Yolun çýkabileceði yere gitmeyin, 					 *
 *	bunun yerine yol olmayan yerlere gidin ve iz býrakýn *
 *	  													 *
 * 									  ~Ralph W. Emerson~ *
 *********************************************************/

#include "stm32f4xx.h"
#include "Mpu6050_i2c.h"
#include "Bmp180.h"
#include "Ds1307.h"

void CLK_Config(){
	RCC-> CR  = 0x00000000;			//Reset Clock Control Register
	RCC-> CR |= 0x01090000;			//HSE On (Write 1 On 5th Step)
									//CSS On (Write 8 On 5th Step)
									//PLL On
	RCC-> PLLCFGR |= 0x07400544;	//HSE Clock Divided By 4 for VCO Input Frequency (PLLM Bits)
//				  		||  ||->	VCO Output Frequency = 336Mhz (336 = VCO Input Frequency(2) * PLLN Bits(168))
//				  		||----->    HSE Selected PLL Source
//				  	  	|------>	USB OTG FS Clock Frequency = 48Mhz (48 = VCO Frequency(336) / PLLQ Bits(7))
	RCC-> CFGR 	  |= 0x00089402;	//PLL Selected As System Clock
//						  |||-->	PPRE1 (APB1 Divided By 4 = 42Mhz)
//						  ||--->	PPRE2 (APB2 Divided By 2 = 84Mhz)
//						  |---->	HSE Clock Divided By 8 For RTC Clock

	RCC-> AHB1ENR |= 0x00040000;	//Backup SRAM Clock Enable

	RCC-> APB1ENR |= 0x10000000;	//Power Interface Clock Enable

	PWR-> CR	  |= 0x0100;		//Backup Access Enable

	RCC-> APB2ENR |= 0x00004000;	//System Configuration Controller Clock Enable
}

void GPIO_Config(){
	RCC->	AHB1ENR |= 0x00000002;	//GPIOB Clock Enable

	GPIOB-> AFR[0]	|= 0x44000000;	//GPIOB 6-7 Set Alternate Function I2C
	GPIOB-> MODER 	|= 0x0000A000;	//GPIOB 6-7 Set as Alternate Function (For I2C1)
	GPIOB-> OTYPER	|= 0x00C0;		//GPIOB 6-7 Set as Open-Drain (7=SDA, 6=SCL)
	GPIOB-> OSPEEDR |= 0x0000F000;	//GPIOB 6-7 Set Very High Speed
	GPIOB-> IDR		 = 0x0000;		//Reset Input Data Register
}

void I2C1_Config(){
	RCC-> APB1ENR |= 0x00200000;	//I2C1 Clock Enable

	I2C1-> CR2	|= 0x002A;		//I2C1 FREQ Set 42Mhz=2A
	I2C1-> CCR	|= 0x00D2;		//I2C1 CCR Set D2
	I2C1-> TRISE|= 0x002B;		//I2C1 TRISE Set 2B
}

void TIM14_Config(){
	RCC-> APB1ENR |= 0x00000100;//Tým14 Clock Enable
	TIM14-> PSC = 0x00A7;		//Tým14 Prescaler is 168;
}

void delay_us(uint32_t delay){
	TIM14-> ARR = delay;
	TIM14-> CR1 = 0x0001;
	if(!(TIM14-> CNT < delay)){
		TIM14-> CR1 = 0x0000;
		TIM14-> CNT = 0;
	}
}

int main(void){

	CLK_Config();
	GPIO_Config();
	I2C1_Config();
	TIM14_Config();



	while (1)
	{

	}
}
