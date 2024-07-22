#include <stm32f4xx.h>
#include "I2C.hpp"

#define HMC_ADDR   0x3C

#define CONF_REG_A 0x00
#define CONF_REG_B 0x01
#define MODE_REG   0x02

#define DATA_X_MSB 0x03
#define DATA_X_LSB 0x04
#define DATA_Y_MSB 0x05
#define DATA_Y_LSB 0x06
#define DATA_Z_MSB 0x07
#define DATA_Z_LSB 0x08

#define STATUS_REG 0x09
#define ID_REG_A   0x0A
#define ID_REG_B   0x0B
#define ID_REG_C   0x0C

//CRA Bits
//MA = Samples Averaged per Measurement Output
#define MA_1       0x00           //1 (def)
#define MA_2       0x20           //2
#define MA_4       0x40           //4
#define MA_8       0x60           //8

//DO = Data Output Rate (Hz)
#define DO_0       0x00           //0.75
#define DO_1       0x04           //1.5
#define DO_2       0x08           //3
#define DO_3       0x0C           //7.5
#define DO_4       0x10           //15 (def)
#define DO_5       0x14           //30
#define DO_6       0x18           //75

//Measurement Mode
#define MS_0       0x00           //Normal Measurement, Resistive pins floating (high-Z) (def)
#define MS_1       0x01           //Positive Bias Configuration for X Y Z axes
#define MS_2       0x02           //Negative Bias Configuration for X Y Z axes

//CRB Bits
//GN = Gain Control Bits
#define GN_0       0x00           // +- 0.88 Ga (1370 LSB/Gauss) (0.73 mG/LSB)
#define GN_1       0x20           // +- 1.3  Ga (1090 LSB/Gauss) (0.92 mG/LSB) (def)
#define GN_2       0x40           // +- 1.9  Ga (820  LSB/Gauss) (1.22 mG/LSB)
#define GN_3       0x60           // +- 2.5  GA (660  LSB/Gauss) (1.52 mg/LSB)
#define GN_4       0x80           // +- 4.0  GA (440  LSB/Gauss) (2.27 mg/LSB)
#define GN_5       0xA0           // +- 4.7  GA (390  LSB/Gauss) (2.56 mg/LSB)
#define GN_6       0xC0           // +- 5.6  GA (330  LSB/Gauss) (3.03 mg/LSB)
#define GN_7       0xE0           // +- 8.1  GA (230  LSB/Gauss) (4.35 mg/LSB)

//Mode Reg Bits
//MD = Mode Select
#define MD_0       0x00           //Continuous Measurement Mode
#define MD_1       0x01           //Single Measurement Mode (def)

class HMC5883 : public I2C_Base{
	public:
	uint8_t  data_rate_val;
	uint16_t gain_val;

	HMC5883(I2C_TypeDef* I2Cxn, uint8_t data_rate, uint8_t gain) :I2C_Base(I2Cxn,STANDART){
		if(gain == GN_0){ gain_val = 1370;}
   else if(gain == GN_1){ gain_val = 1090;}
   else if(gain == GN_2){ gain_val =  820;}
   else if(gain == GN_3){ gain_val =  660;}
   else if(gain == GN_4){ gain_val =  440;}
   else if(gain == GN_5){ gain_val =  390;}
   else if(gain == GN_6){ gain_val =  330;}
   else if(gain == GN_7){ gain_val =  230;}
	}

	void config();

};
