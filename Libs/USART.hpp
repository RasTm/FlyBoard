#pragma once
#include "stm32f4xx.h"
#include "GPIO.hpp"
#include <vector>
#include <stdio.h>

#define USART_1 1
#define USART_2 2
#define USART_3 3
#define UART_4  4
#define UART_5  5
#define USART_6 6

#define Periph_clk 0

class USART_Base{
	public:
	USART_TypeDef *USARTx;

	float Usart_Div;

	uint16_t Div_Mantissa;
	uint8_t Div_Fraction;

	USART_Base(uint8_t USARTxn, uint32_t target_baud = 9600){
		if(USARTxn == USART_1){
			USARTx = ((USART_TypeDef*) USART1_BASE);
			RCC-> APB2ENR |= 0x00000010;
			Set_Gpio(GPIOA,  9, ALTER, PUSH, LOW, NOT, USART1_3);		//TX
			Set_Gpio(GPIOA, 10, ALTER, PUSH, LOW, NOT, USART1_3);		//RX
			#undef Periph_clk
			#define Periph_clk 84000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == USART_2){
			USARTx = ((USART_TypeDef*) USART2_BASE);
			RCC-> APB1ENR |= 0x00020000;
			Set_Gpio(GPIOD, 5, ALTER, PUSH, LOW, NOT, USART1_3);		//TX
			Set_Gpio(GPIOD, 6, ALTER, PUSH, LOW, NOT, USART1_3);		//RX
			#undef Periph_clk
			#define Periph_clk 42000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == USART_3){
			USARTx = ((USART_TypeDef*) USART3_BASE);
			RCC-> APB1ENR |= 0x00040000;
			Set_Gpio(GPIOB, 10, ALTER, PUSH, LOW, NOT, USART1_3);		//TX
			Set_Gpio(GPIOB, 11, ALTER, PUSH, LOW, NOT, USART1_3);		//RX
			#undef Periph_clk
			#define Periph_clk 42000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == UART_4){
			USARTx = ((USART_TypeDef*) UART4_BASE);
			RCC-> APB1ENR |= 0x00080000;
			Set_Gpio(GPIOA, 0, ALTER, PUSH, LOW, NOT, USART4_6);		//TX
			Set_Gpio(GPIOA, 1, ALTER, PUSH, LOW, NOT, USART4_6);		//RX
			#undef Periph_clk
			#define Periph_clk 42000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == UART_5){
			USARTx = ((USART_TypeDef*) UART5_BASE);
			RCC-> APB1ENR |= 0x00100000;
			Set_Gpio(GPIOC, 12, ALTER, PUSH, LOW, NOT, USART4_6);		//TX
			Set_Gpio(GPIOD,  2, ALTER, PUSH, LOW, NOT, USART4_6);		//RX
			#undef Periph_clk
			#define Periph_clk 42000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == USART_6){
			USARTx = ((USART_TypeDef*) USART6_BASE);
			RCC-> APB2ENR |= 0x00000020;
			Set_Gpio(GPIOC, 6, ALTER, PUSH, LOW, NOT, USART4_6);		//TX
			Set_Gpio(GPIOC, 7, ALTER, PUSH, LOW, NOT, USART4_6);		//RX
			#undef Periph_clk
			#define Periph_clk 84000000							//Later (APB1 or APB2 clocks are writing)
		}

		Usart_Div = ((float)Periph_clk/(16*target_baud));		//
		Div_Mantissa = (uint16_t)Usart_Div;						//
																//	Calculate USART Baud Timings
		Usart_Div = Usart_Div - (float)Div_Mantissa;			//
		Div_Fraction = Usart_Div*16;							//

		USARTx-> BRR |= (Div_Mantissa<<4)+Div_Fraction;			//	Write to the USART_BAUDRATE Register
	}

	template <typename T> void USART_Transmit(std::vector<T> &data);
	void USART_Transmit(uint8_t *data, uint16_t size);
	void USART_Transmit(uint8_t *data);
	void USART_Transmit(char *data);
	void USART_Transmit_float(float data, uint8_t lenght);

	void USART_Receive(std::vector<uint8_t> data);
	void USART_Receive(uint8_t *data, uint8_t size);
};

template <typename T> void USART_Base::USART_Transmit(std::vector<T> &data){

	uint8_t data_size = sizeof(data[0]);

	USARTx-> CR1  = 0;							//Reset USART CR1 Register
	USARTx-> CR1 |= 0x00002008;					//USART Enable TX Mode Set

	if(data_size == 1){
		for(T Usart_data : data){
			while(!(USARTx-> SR & 0x0000080));	//Check TXE bit
			USARTx-> DR = Usart_data;
		}
	}
	else if(data_size == 2){
		for(T Usart_data : data){
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0x00FF);
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0xFF00);
		}
	}
	else if(data_size == 4){
		for(T Usart_data : data){
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0x000000FF);
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0x0000FF00);
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0x00FF0000);
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0xFF000000);
		}
	}

	while(!(USARTx-> SR & 0x000000040));	//Wait Until Transmission Complete Bit Set
	USARTx-> CR1 &= 0xFFFFFFF7; 			//USART Disable TX Mode Clear
}
