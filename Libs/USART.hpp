#pragma once
#include "stm32f4xx.h"
#include "GPIO.hpp"
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>

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
			gpio_config(GPIOA,  9, ALTER, PUSH, MED, NOT, USART1_3);	//TX
			gpio_config(GPIOA, 10, ALTER, PUSH, MED, UP, USART1_3);		//RX
			#undef Periph_clk
			#define Periph_clk 84000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == USART_2){
			USARTx = ((USART_TypeDef*) USART2_BASE);
			RCC-> APB1ENR |= 0x00020000;
			gpio_config(GPIOD, 5, ALTER, PUSH, MED, NOT, USART1_3);		//TX
			gpio_config(GPIOD, 6, ALTER, PUSH, MED, UP, USART1_3);		//RX
			#undef Periph_clk
			#define Periph_clk 42000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == USART_3){
			USARTx = ((USART_TypeDef*) USART3_BASE);
			RCC-> APB1ENR |= 0x00040000;
			gpio_config(GPIOB, 10, ALTER, PUSH, MED, NOT, USART1_3);	//TX
			gpio_config(GPIOB, 11, ALTER, PUSH, MED, UP, USART1_3);		//RX
			#undef Periph_clk
			#define Periph_clk 42000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == UART_4){
			USARTx = ((USART_TypeDef*) UART4_BASE);
			RCC-> APB1ENR |= 0x00080000;
			gpio_config(GPIOA, 0, ALTER, PUSH, MED, NOT, USART4_6);		//TX
			gpio_config(GPIOA, 1, ALTER, PUSH, MED, UP, USART4_6);		//RX
			#undef Periph_clk
			#define Periph_clk 42000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == UART_5){
			USARTx = ((USART_TypeDef*) UART5_BASE);
			RCC-> APB1ENR |= 0x00100000;
			gpio_config(GPIOC, 12, ALTER, PUSH, MED, NOT, USART4_6);	//TX
			gpio_config(GPIOD,  2, ALTER, PUSH, MED, UP, USART4_6);		//RX
			#undef Periph_clk
			#define Periph_clk 42000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == USART_6){
			USARTx = ((USART_TypeDef*) USART6_BASE);
			RCC-> APB2ENR |= 0x00000020;
			gpio_config(GPIOC, 6, ALTER, PUSH, MED, NOT, USART4_6);		//TX
			gpio_config(GPIOC, 7, ALTER, PUSH, MED, UP, USART4_6);		//RX
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

	static void USART_enable_TXE_IRQ(USART_TypeDef *USARTxn);
	static void USART_disable_TXE_IRQ(USART_TypeDef *USARTxn);
	static void USART_enable_TC_IRQ(USART_TypeDef *USARTxn);
	static void USART_disable_TC_IRQ(USART_TypeDef *USARTxn);
	static void USART_enable_RXNEIE_IRQ(USART_TypeDef *USARTxn);
	static void USART_disable_RXNEIE_IRQ(USART_TypeDef *USARTxn);
	static void USART_enable_IDLE_IRQ(USART_TypeDef *USARTxn);
	static void USART_disable_IDLE_IRQ(USART_TypeDef *USARTxn);

	template<typename T> void Transmit(T &value);
	void USART_Transmit(uint8_t *data, uint16_t size);
	void USART_Transmit(const uint8_t *data);
	void USART_Transmit(const char *data);
	void USART_Transmit_float(float data, uint8_t lenght);

	void USART_Receive(std::vector<uint8_t> data);
	void USART_Receive(uint8_t *data, uint8_t size);
};

template<typename T> void USART_Base::Transmit(T &value){
	char buffer[32];

    if (std::is_integral<T>::value) {
        snprintf(buffer, sizeof(buffer), "%ld", static_cast<long>(value));
    }
    //(floating point types)
    else if (std::is_floating_point<T>::value) {
        snprintf(buffer, sizeof(buffer), "%.2f", static_cast<double>(value));
    }

	USART_Transmit((uint8_t*)buffer);
}
/*
template<typename T> void USART_Base::Transmit(T &value){
	std::stringstream ss;
	ss << value;
	std::string str = ss.str();
	USART_Transmit(str.c_str());
}*/
