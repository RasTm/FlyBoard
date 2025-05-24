#include "stm32f4xx.h"
#include "GPIO.hpp"
#include <bitset>

#define ADC_1 1
#define ADC_2 2
#define ADC_3 3

uint8_t sequence=0;

class ADC_Base{
	private:
	ADC_TypeDef *ADCx;
	public:

	ADC_Base(uint8_t ADCX, uint16_t Channel, uint8_t Sample=0){

		if(ADCX == ADC_1){												//ADC Definition And RCC Settings																	//
			ADCx = ((ADC_TypeDef *) ADC1_BASE);							//
			RCC-> APB2ENR |= 0x00000100;								//
			ADCx-> CR2 |= 0x00000001;									//ADC On
		}
		else if(ADCX == ADC_2){											//
			ADCx = ((ADC_TypeDef *) ADC2_BASE);							//
			RCC-> APB2ENR |= 0x00000200;								//
			ADCx-> CR2 |= 0x00000001;									//ADC On
		}
		else if(ADCX == ADC_3){											//
			ADCx = ((ADC_TypeDef *) ADC3_BASE);							//
			RCC-> APB2ENR |= 0x00000400;								//
			ADCx-> CR2 |= 0x00000001;									//ADC On
		}

		ADC-> CCR |= 0x00010000;										//ADC Prescaler set to 4 (84/4 = 21 MHz)

		std::bitset<16> CH(Channel);

		for(uint8_t i=0; i<16; i++){									//ADC Pin Configuration And SQR Register Settings
			if(i<8){
				if(CH[i] == 1){
					gpio_config(GPIOA, i, ANALOG, PUSH, MED, NOT, SyS);		//ADC_IN0 to ADC_IN7 allocated in GPIOA Pin 0 to 7
					if(sequence<6){
						ADCx-> SQR3 |= (i<<(sequence*5));				//ADC Sequence Register Settings
						sequence += 1;
					}
					else if(sequence>6 && sequence<12){
						ADCx-> SQR2 |= (i<<((sequence-7)*5));			//ADC Sequence Register Settings
						sequence += 1;
					}
					else{
						ADCx-> SQR1 = (i<<((sequence-13)*5));			//ADC Sequence Register Settings
						sequence += 1;
					}
				}
			}
			else if(i>7 && i<10){
				if(CH[i] == 1){
					gpio_config(GPIOB, i-8, ANALOG, PUSH, MED, NOT, SyS);	//ADC_IN8 And ADC_IN9 allocated in GPIOB Pin 0 And 1
					if(sequence<6){
						ADCx-> SQR3 |= (i<<(sequence*5));				//ADC Sequence Register Settings
						sequence += 1;
					}
					else if(sequence>6 && sequence<12){
						ADCx-> SQR2 |= (i<<((sequence-7)*5));			//ADC Sequence Register Settings
						sequence += 1;
					}
					else{
						ADCx-> SQR1 |= (i<<((sequence-13)*5));			//ADC Sequence Register Settings
						sequence += 1;
					}
				}
			}
			else{
				if(CH[i] == 1){
					gpio_config(GPIOC, i-10, ANALOG, PUSH, MED, NOT, SyS);	//ADC_IN10 to ADC_IN15 allocated in GPIOC Pin 0 to 5
					if(sequence<6){
						ADCx-> SQR3 |= (i<<(sequence*5));				//ADC Sequence Register Settings
						sequence += 1;
					}
					else if(sequence>6 && sequence<12){
						ADCx-> SQR2 |= (i<<((sequence-7)*5));			//ADC Sequence Register Settings
						sequence += 1;
					}
					else{
						ADCx-> SQR1 |= (i<<((sequence-13)*5));			//ADC Sequence Register Settings
						sequence += 1;
					}
				}
			}
		}																//End of Pin Configuration And SQR Register Settings

		if(Channel > 9){ ADCx-> SMPR1 |= (Sample<<((Channel-10)*3)); }	//ADC Sample Rate Settings
		else{ 			 ADCx-> SMPR2 |= (Sample<<(Channel*3));      }	//

		ADCx-> SQR1 = (CH.count()<<20);									//ADC Conversion Number

	}

	void ADC_Start();
	uint16_t ADC_Single_Conv();

};

