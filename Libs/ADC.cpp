#include "ADC.hpp"

uint8_t ADC_Base::adc_ch_count[3] = {0, 0, 0};

void ADC_Base::ADC_dma_enable(ADC_TypeDef *ADCxn){
	ADCxn-> CR2 |= 0x00000300;                        //ADC DMA Request Enable In Every Conversion
}

void ADC_Base::ADC_enable_IRQ(ADC_TypeDef *ADCxn){
	ADCxn-> CR1 |= 0x00000020;                        //ADC EOC IRQ Enable
}

void ADC_Base::ADC_disable_IRQ(ADC_TypeDef *ADCxn){
	ADCxn-> CR1 &= 0xFFFFFFDF;                        //ADC EOC IRQ Disable
}

void ADC_Base::ADC_scan_enable(ADC_TypeDef *ADCxn){
	ADCxn-> CR1 |= 0x00000100;                        //ADC Scan Mode Enable
}

void ADC_Base::ADC_scan_disable(ADC_TypeDef *ADCxn){
	ADCxn-> CR1 &= 0xFFFFFEFF;                        //ADC Scan Mode Disable
}

void ADC_Base::ADC_continuous_enable(ADC_TypeDef *ADCxn){
	ADCxn-> CR2 |= 0x00000002;                        //ADC Continuous Mode Enable
}

void ADC_Base::ADC_continuous_disable(ADC_TypeDef *ADCxn){
	ADCxn-> CR2 &= 0xFFFFFFFD;                        //ADC Continuous Mode Disable
}

void ADC_Base::ADC_start(ADC_TypeDef *ADCxn){
	ADCxn-> CR2 |= 0x40000000;                        //ADC Regular Conversion Start
}

uint16_t ADC_Base::ADC_single_conv(){
	ADCx-> CR2 |= 0x40000000;
	while(!(ADCx-> SR & 0x00000010));                //ADC Regular Channel Conversion Start Flag Check
	while(!(ADCx-> SR & 0x00000002));                //ADC Regular Channel End Of Conversion Flag Check
	ADCx-> CR2 &= 0xBFFFFFFF;                        //ADC Regular Channel Conversion Start Bit Clear
	return (ADCx-> DR);
}
