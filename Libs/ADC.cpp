#include "ADC.hpp"

void ADC_Base::ADC_Start(){
	ADCx-> CR2 |= 0x40000000;
}

uint16_t ADC_Base::ADC_Single_Conv(){
	ADCx-> CR2 |= 0x40000000;
	while(!(ADCx-> SR & 0x00000010));									//ADC Regular Channel Conversion Start Flag Check
	while(!(ADCx-> SR & 0x00000002));									//ADC Regular Channel End Of Conversion Flag Check
	ADCx-> CR2 &= 0xBFFFFFFF;											//ADC Regular Channel Conversion Start Bit Clear
	return (ADCx-> DR);
}
