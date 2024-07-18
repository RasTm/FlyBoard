#include "SPI.hpp"

/**
  * @brief  This method allows you to transmit 16 bit data through SPI interface.
  * @param  data        : This variable is what do you want to transmit.
  * @retval -
  */
void SPI_Base::write_byte_SPI(uint16_t data){
	SPIx-> CR1 |= 0x0040;               //SPI Enable
//	SPIx-> CR2 |= 0x0004;               //SPI SSOE set

	while(!(SPIx-> SR & 0x0002));       //Checking TXE bit (Transmit Buffer Empty)
	SPIx-> DR   = data;
    while(!(SPIx-> SR & 0x0002));       //Checking TXE bit (Transmit Buffer Empty)
    while(!(SPIx-> SR & 0x0080)){}      //Checking BSY bit (Busy)

	SPIx-> CR1 &= 0xFFBF;				//Clearing SPE Bit
//	SPIx-> CR2 &= 0xFFFB;				//Clearing SSOE Bit
	uint8_t a = 250;
    while(a){a -= 1;}
}

void SPI_Base::multi_byte_write_SPI(std::vector<uint16_t> &data){
	for(auto x : data){
        SPIx-> CR1 |= 0x0040;               //SPI Enable

        while(!(SPIx-> SR & 0x0002)){}      //Checking TXE bit (Transmit Buffer Empty)
	    SPIx-> DR   = x;
        while(!(SPIx-> SR & 0x0002));       //Checking TXE bit (Transmit Buffer Empty)
        while(!(SPIx-> SR & 0x0080)){}      //Checking BSY bit (Busy)

	    SPIx-> CR1 &= 0xFFBF;				//Clearing SPE Bit
	    uint8_t a = 210;
	    while(a){a -= 1;}
	}
}
