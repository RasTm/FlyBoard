#include "stm32f4xx.h"
#include "GPIO.hpp"
#include <bitset>

#define ADC_1 1
#define ADC_2 2
#define ADC_3 3

class ADC_Base{
	private:
	ADC_TypeDef *ADCx;
	uint8_t active = 0;
	public:
	static uint8_t adc_ch_count[3];
	ADC_Base(uint8_t ADCX, uint8_t Channel, uint8_t Sequence, uint8_t Sample){
		if(ADCX == ADC_1){												//ADC Definition And RCC Settings																	//
			ADCx = ((ADC_TypeDef *) ADC1_BASE);							//
			RCC-> APB2ENR |= 0x00000100;								//ADC1 Clock Enable
			ADCx-> CR2 |= 0x00000001;									//ADC On
			adc_ch_count[0]++;
			active = 0;
		}
		else if(ADCX == ADC_2){											//
			ADCx = ((ADC_TypeDef *) ADC2_BASE);							//
			RCC-> APB2ENR |= 0x00000200;								//ADC2 Clock Enable
			ADCx-> CR2 |= 0x00000001;									//ADC On
			adc_ch_count[1]++;
			active = 1;
		}
		else if(ADCX == ADC_3){											//
			ADCx = ((ADC_TypeDef *) ADC3_BASE);							//
			RCC-> APB2ENR |= 0x00000400;								//ADC3 Clock Enable
			ADCx-> CR2 |= 0x00000001;									//ADC On
			adc_ch_count[2]++;
			active = 2;
		}

		ADC-> CCR |= 0x00010000;										//ADC Prescaler set to 4 (84/4 = 21 MHz)

/*		std::bitset<16> CH(Channel);

		for(uint8_t i=0; i<16; i++){									//ADC Pin Configuration And SQR Register Settings
			if(i<8){
				if(CH[i] == 1){
					gpio_config(GPIOA, i, ANALOG, PUSH, MED, NOT, SyS);		//ADC_IN0 to ADC_IN7 allocated in GPIOA Pin 0 to 7
					if(Sequence<6){
						ADCx-> SQR3 |= (i<<(Sequence*5));				//ADC Sequence Register Settings
						Sequence += 1;
					}
					else if(Sequence>6 && Sequence<12){
						ADCx-> SQR2 |= (i<<((Sequence-7)*5));			//ADC Sequence Register Settings
						Sequence += 1;
					}
					else{
						ADCx-> SQR1 = (i<<((Sequence-13)*5));			//ADC Sequence Register Settings
						Sequence += 1;
					}
				}
			}
			else if(i>7 && i<10){
				if(CH[i] == 1){
					gpio_config(GPIOB, i-8, ANALOG, PUSH, MED, NOT, SyS);	//ADC_IN8 And ADC_IN9 allocated in GPIOB Pin 0 And 1
					if(Sequence<6){
						ADCx-> SQR3 |= (i<<(Sequence*5));				//ADC Sequence Register Settings
						Sequence += 1;
					}
					else if(Sequence>6 && Sequence<12){
						ADCx-> SQR2 |= (i<<((Sequence-7)*5));			//ADC Sequence Register Settings
						Sequence += 1;
					}
					else{
						ADCx-> SQR1 |= (i<<((Sequence-13)*5));			//ADC Sequence Register Settings
						Sequence += 1;
					}
				}
			}
			else{
				if(CH[i] == 1){
					gpio_config(GPIOC, i-10, ANALOG, PUSH, MED, NOT, SyS);	//ADC_IN10 to ADC_IN15 allocated in GPIOC Pin 0 to 5
					if(Sequence<6){
						ADCx-> SQR3 |= (i<<(Sequence*5));				//ADC Sequence Register Settings
						Sequence += 1;
					}
					else if(Sequence>6 && Sequence<12){
						ADCx-> SQR2 |= (i<<((Sequence-7)*5));			//ADC Sequence Register Settings
						Sequence += 1;
					}
					else{
						ADCx-> SQR1 |= (i<<((Sequence-13)*5));			//ADC Sequence Register Settings
						Sequence += 1;
					}
				}
			}
		}*/																//End of Pin Configuration And SQR Register Settings

		if(Sequence<6){
			ADCx-> SQR3 |= (Channel<<(Sequence*5));				//ADC Sequence Register Settings
		}
		else if(Sequence>6 && Sequence<12){
			ADCx-> SQR2 |= (Channel<<((Sequence-7)*5));			//ADC Sequence Register Settings
		}
		else{
			ADCx-> SQR1 |= (Channel<<((Sequence-13)*5));			//ADC Sequence Register Settings
		}

		if(Channel < 8){
			gpio_config(GPIOA, Channel, ANALOG, PUSH, MED, NOT, SyS);		//ADC_IN0 to ADC_IN7 allocated in GPIOA Pin 0 to 7
		}
		else if(Channel > 7 && Channel < 10){
			gpio_config(GPIOB, Channel-8, ANALOG, PUSH, MED, NOT, SyS);     //ADC_IN8 And ADC_IN9 allocated in GPIOB Pin 0 And 1
		}
		else{
			gpio_config(GPIOC, Channel-10, ANALOG, PUSH, MED, NOT, SyS);    //ADC_IN10 to ADC_IN15 allocated in GPIOC Pin 0 to 5
		}

		if(Channel > 9){ ADCx-> SMPR1 |= (Sample<<((Channel-10)*3)); }	//ADC Sample Rate Settings
		else{ 			 ADCx-> SMPR2 |= (Sample<<(Channel*3));      }	//

		ADCx-> SQR1 &= 0xFF0FFFFF;                                      //ADC Sequence Length Clear
		ADCx-> SQR1 |= (adc_ch_count[active]<<20);						        //ADC Sequence Length Number
	}
    static void ADC_enable_IRQ(ADC_TypeDef *ADCxn);
    static void ADC_scan_enable(ADC_TypeDef *ADCxn);
    static void ADC_continuous_enable(ADC_TypeDef *ADCxn);
	static void ADC_start(ADC_TypeDef *ADCxn);
	uint16_t ADC_single_conv();
};

