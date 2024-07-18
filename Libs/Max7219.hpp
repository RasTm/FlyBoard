#pragma once
#include "SPI.hpp"
#include "Timers.hpp"

#define NOOP 		0x0000
#define Digit0 		0x0100
#define Digit1 		0x0200
#define Digit2 		0x0300
#define Digit3 		0x0400
#define Digit4 		0x0500
#define Digit5 		0x0600
#define Digit6 		0x0700
#define Digit7 		0x0800
#define DECODE_MODE 0x0900
#define INTENSITY 	0x0A00
#define SCANLIMIT 	0x0B00
#define SHUTDOWN 	0x0C00
#define TEST 		0x0F00

class MAX7219 : public SPI_Base{
	public:
	MAX7219(SPI_TypeDef *SPIxn):SPI_Base(SPIxn, TRANSMIT_ONLY_MASTER, HARDWARE_NSS_OUTPUT, 64, 1, 0){
	    write_byte_SPI(0x0F00);            //Display Test Register Normal Operation Mode
		//delay(1);
		write_byte_SPI(0x0900);            //Decode Mode Register Set To No Decode
		//delay(1);
		write_byte_SPI(0x0A0F);            //Intensity Register Full Brightness
		//delay(1);
		write_byte_SPI(0x0B07);            //Scan-Limit Register Set To Full Scan
	    //delay(1);
		write_byte_SPI(0x0C01);            //Shutdown Register Set To Normal Operation
		//delay(1);
	}
    void clear_disp();
    void send_char();
};
