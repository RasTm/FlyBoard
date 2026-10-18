#include "stm32f4xx.h"
#include "USART.hpp"

#define GPS_BUFFER_SIZE 1024

class NEOM8:public USART_Base{
    public:
	uint8_t nmea[GPS_BUFFER_SIZE]={0},count=0;
	NEOM8(uint8_t USARTxn, uint32_t target_baud = 9600):USART_Base(USARTxn,target_baud){

	}
	void parse_gps();
};
