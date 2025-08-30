#include "RCC.hpp"

void Clock_init(){
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
        SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
    #endif

    RCC->CR |= (uint32_t)0x00000001;//Set HSION Bit

    RCC->CFGR = 0x00000000;         //Reset CFGR Register

    RCC->CR &= (uint32_t)0xFEF6FFFF;//Reset HSEON, CSSON and PLLON Bits

    RCC->PLLCFGR = 0x24003010;      //Reset PLLCFGR Register

    RCC->CR &= (uint32_t)0xFFFBFFFF;//Reset HSEBYP Bit

    RCC->CIR = 0x00000000;          //Disable All Interrupts

//	RCC-> CR  = 0x00000000;			//Reset Clock Control Register
	RCC-> CR |= 0x00010000;         //HSE On (Write 1 On 5th Step)
	while(!(RCC-> CR & 0x00020000));//Wait For HSE Ready Flag Set

    RCC->APB1ENR |= RCC_APB1ENR_PWREN; //Select regulator voltage output Scale 1 mode, System frequency up to 168 MHz
    PWR->CR |= PWR_CR_VOS;

//	RCC-> PLLCFGR  = 0;
	RCC-> CFGR 	  |= 0x00089400;
//						  |||-->	PPRE1 (APB1 Divided By 4 = 42Mhz)
//						  ||--->	PPRE2 (APB2 Divided By 2 = 84Mhz)
//						  |---->	HSE Clock Divided By 8 For RTC Clock

	RCC-> PLLCFGR  = 0x07405408;	//HSE Clock Divided By 8 for VCO Input Frequency (PLLM Bits)
//				  		|| ||-->	VCO Output Frequency = 336Mhz (336 = VCO Input Frequency(1) * PLLN Bits(336))
//				  		||----->    HSE Selected PLL Source
//				  	  	|------>	USB OTG FS Clock Frequency = 48Mhz (48 = VCO Frequency(336) / PLLQ Bits(7))

	RCC-> CR |= 0x01000000;			//PLL On (Write 1 On 7th Step)
	while(!(RCC-> CR & 0x02000000));//Wait For PLL Ready Flag Set
	RCC-> CR |= 0x00080000;         //CSS On (Write 8 On 5th Step)

	FLASH->ACR = FLASH_ACR_PRFTEN |FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

	RCC-> CFGR    |= 0x00000002;    //PLL Selected As System Clock

	RCC-> AHB1ENR |= 0x00040000;	//Backup SRAM Clock Enable

	RCC-> APB1ENR |= 0x10000000;	//Power Interface Clock Enable

	PWR-> CR	  |= 0x0100;		//Backup Access Enable

	RCC-> APB2ENR |= 0x00004000;	//System Configuration Controller Clock Enable

	RCC-> AHB1ENR |= 0x00600000;    //DMA1 and DMA2 Clock Enable

	RCC-> AHB1ENR |= 0x000001FF;	//All GPIO Port Clocks Are Enable

    DBGMCU-> APB1FZ |= 0x00000080;  //TIM13 Counter Stop When Core Halted

	SysTick-> LOAD = 0;
	SysTick-> LOAD = 167999;		//Systick Load is 167,999 (168 Million Pulse each second/ 167,999 pulse = 1 millisecond)

	SysTick-> CTRL|= 0x00000007;	//Systick Clock Source AHB(Processor Clock)
}

/**
  * @brief  This function allows you to save array data to the RTC_BKP registers.
  * @note   STM32F4xx has 32 bits per RTC_BKPx (x=1,2,etc) register.
  *         Soo you can save 32x20/8 = 80 bytes of data.
  * @param  text: This pointer holds your data to be save
  * @param  n: This variable holds size (bytes) of your data
  *
  * @retval -
  */
void RTC_BKP_Write(uint8_t* text, uint8_t n){
	RCC-> BDCR 	= 0x00008300;	//Backup Domain Control RTC Clock Enable, HSE Selected as RTC Clock Source
	RTC-> WPR	= 0xCA;			//Key_1
	RTC-> WPR 	= 0x53;			//Key_2

	uint8_t j=0;
    volatile uint32_t* pointer;
    pointer = reinterpret_cast<uint32_t*> (0x40002850);		//Address of RTC-> BKP0R
    *pointer = 0;

    for(uint8_t i=0; i<n; ++i){
    	uint8_t data = text[i];
        *pointer |= (data<<(24-(j*8)));
        j++;
        if(j==4){
        	if(pointer == &(RTC-> BKP19R)){
        		break;
        	}
            pointer = reinterpret_cast<uint32_t*> (0x40002850 + (0x04 * ((i+1)/4) ));	//Address of RTC-> BKP0R + Offset
            *pointer = 0;
            j=0;
        }
    }
//	RCC-> BDCR = 0x00010000;	//Backup Domain Control Disabled
}

/**
  * @brief  This function allows you to read data from the RTC_BKP registers.
  * @param  data: This pointer holds your data to be read
  * @retval -
  */
void RTC_BKP_Read(uint8_t* data){
	RCC-> BDCR 	= 0x00008300;	//Backup Domain Control RTC Clock Enable, HSE Selected as RTC Clock Source
	RTC-> WPR	= 0xCA;			//Key_1
	RTC-> WPR 	= 0x53;			//Key_2

//	volatile uint32_t* pointer;

}
