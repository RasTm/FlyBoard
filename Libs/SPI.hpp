#include "stm32f4xx.h"
#include "GPIO.hpp"
#include <cmath>
#include <vector>

#define FULL_DUPLEX_MASTER    1
#define FULL_DUPLEX_SLAVE     2

#define HALF_DUPLEX_MASTER    3
#define HALF_DUPLEX_SLAVE     4

#define RECEIVE_ONLY_MASTER   5
#define RECEIVE_ONLY_SLAVE    6

#define TRANSMIT_ONLY_MASTER  7
#define TRANSMIT_ONLY_SLAVE   8

#define NSS_DISABLE           9
#define HARDWARE_NSS_INPUT    10
#define HARDWARE_NSS_OUTPUT   11

class SPI_Base{
	private:

	public:
	SPI_TypeDef *SPIx;
	SPI_Base(SPI_TypeDef *SPIxn, uint8_t Mode, uint8_t NSS_Mode, uint8_t baud_rate_div, bool DFF, bool LSBFirst){

		if(SPIxn == SPI1){
			SPIx = ((SPI_TypeDef *) SPI1_BASE);
			RCC-> APB2ENR |= 0x00001000;                                    //SPI1 Clock Enable
			Set_Gpio(GPIOA, 5, ALTER, PUSH, HIGH, NOT, SPI1_2);             //SCK Pin

			if(NSS_Mode != NSS_DISABLE){
				Set_Gpio(GPIOA, 4, ALTER, PUSH, HIGH, UP, SPI1_2);          //NSS Pin
			}
			if(Mode == FULL_DUPLEX_MASTER || Mode == FULL_DUPLEX_SLAVE){
				Set_Gpio(GPIOA, 6, ALTER, PUSH, HIGH, UP, SPI1_2);          //MISO Pin
				Set_Gpio(GPIOA, 7, ALTER, PUSH, HIGH, NOT, SPI1_2);         //MOSI Pin
			}
			else if(Mode == HALF_DUPLEX_MASTER || Mode == RECEIVE_ONLY_SLAVE  || Mode == TRANSMIT_ONLY_MASTER){
				Set_Gpio(GPIOA, 7, ALTER, PUSH, HIGH, NOT, SPI1_2);         //MOSI Pin
				SPIx-> CR1 |= 0x8000;                                       //SPI BIDIMODE Enable
			}
			else if(Mode == HALF_DUPLEX_SLAVE  || Mode == RECEIVE_ONLY_MASTER || Mode == TRANSMIT_ONLY_SLAVE){
				Set_Gpio(GPIOA, 6, ALTER, PUSH, HIGH, UP, SPI1_2);          //MISO Pin
				SPIx-> CR1 |= 0x8000;                                       //SPI BIDIMODE Enable
			}
		}

		else if(SPIxn == SPI2){
			SPIx = ((SPI_TypeDef *) SPI2_BASE);
			RCC-> APB1ENR |= 0x00004000;                                    //SPI2 Clock Enable
			Set_Gpio(GPIOB, 10, ALTER, PUSH, HIGH, NOT, SPI1_2);            //SCK Pin

			if(NSS_Mode != NSS_DISABLE){
				Set_Gpio(GPIOB, 12, ALTER, PUSH, HIGH, UP, SPI1_2);         //NSS Pin
			}
			if(Mode == FULL_DUPLEX_MASTER || Mode == FULL_DUPLEX_SLAVE){
				Set_Gpio(GPIOC, 2, ALTER, PUSH, HIGH, UP, SPI1_2);          //MISO Pin
				Set_Gpio(GPIOC, 3, ALTER, PUSH, HIGH, NOT, SPI1_2);         //MOSI Pin
			}
			else if(Mode == HALF_DUPLEX_MASTER || Mode == RECEIVE_ONLY_SLAVE  || Mode == TRANSMIT_ONLY_MASTER){
				Set_Gpio(GPIOC, 3, ALTER, PUSH, HIGH, NOT, SPI1_2);         //MOSI Pin
				SPIx-> CR1 |= 0x8000;                                       //SPI BIDIMODE Enable
			}
			else if(Mode == HALF_DUPLEX_SLAVE  || Mode == RECEIVE_ONLY_MASTER || Mode == TRANSMIT_ONLY_SLAVE){
				Set_Gpio(GPIOC, 2, ALTER, PUSH, HIGH, UP, SPI1_2);          //MISO Pin
				SPIx-> CR1 |= 0x8000;                                       //SPI BIDIMODE Enable
			}
		}

		else if(SPIxn == SPI3){
			SPIx = ((SPI_TypeDef *) SPI3_BASE);
			RCC-> APB1ENR |= 0x00008000;                                     //SPI3 Clock Enable
			Set_Gpio(GPIOC, 10, ALTER, PUSH, HIGH, NOT, SPI_3);              //SCK Pin

			if(NSS_Mode != NSS_DISABLE){
				Set_Gpio(GPIOA, 15, ALTER, PUSH, HIGH, UP, SPI_3);           //NSS Pin
			}
			if(Mode == FULL_DUPLEX_MASTER || Mode == FULL_DUPLEX_SLAVE){
				Set_Gpio(GPIOC, 11, ALTER, PUSH, HIGH, UP, SPI_3);           //MISO Pin
				Set_Gpio(GPIOC, 12, ALTER, PUSH, HIGH, NOT, SPI_3);          //MOSI Pin
			}
			else if(Mode == HALF_DUPLEX_MASTER || Mode == RECEIVE_ONLY_SLAVE  || Mode == TRANSMIT_ONLY_MASTER){
				Set_Gpio(GPIOC, 12, ALTER, PUSH, HIGH, NOT, SPI_3);          //MOSI Pin
				SPIx-> CR1 |= 0x8000;                                        //SPI BIDIMODE Enable
			}
			else if(Mode == HALF_DUPLEX_SLAVE  || Mode == RECEIVE_ONLY_MASTER || Mode == TRANSMIT_ONLY_SLAVE){
				Set_Gpio(GPIOC, 11, ALTER, PUSH, HIGH, UP, SPI_3);           //MISO Pin
				SPIx-> CR1 |= 0x8000;                                        //SPI BIDIMODE Enable
			}
		}
		SPIx-> CR1 |= ((uint16_t)( log(baud_rate_div) / log(2) )-1)<<3;      //SPI SCK Freq
		SPIx-> CR1 |= DFF << 11;                                             //Data Frame Format Selection
		SPIx-> CR1 |= LSBFirst << 7;                                         //LSB First Bit Selection

		if(NSS_Mode == NSS_DISABLE){
			SPIx-> CR1 |= 0x0300;                                            //SPI Software Slave Select and Internal Slave Select Bits Set
		}
		else if(NSS_Mode == HARDWARE_NSS_OUTPUT){
			SPIx-> CR2 |= 0x0004;                                            //SPI Slave Select Output Enable (Device cannot work only Multimaster env.)
		}

		if(Mode == TRANSMIT_ONLY_MASTER || Mode == TRANSMIT_ONLY_SLAVE){
			SPIx-> CR1 |= 0x4000;                                            //SPI Set Transmit Only Mode
		}

		if(Mode == TRANSMIT_ONLY_MASTER || Mode == FULL_DUPLEX_MASTER || Mode == HALF_DUPLEX_MASTER){
			SPIx-> CR1 |= 0x0004;                                            //SPI Master Mode Set
		}

	}
	void write_byte_SPI(uint16_t data);
	void multi_byte_write_SPI(std::vector<uint16_t> &data);

};
