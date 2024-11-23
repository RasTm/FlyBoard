#include <stm32f4xx.h>
#include "I2C.hpp"

#define RAD_TO_DEG 57.295779513082320876798154814105
#define DEC_ANGLE (6.0 + (14.0/60.0)) / (57.295779513082320876798154814105)

#ifndef PI
#define PI					3.14159265358979f
#endif

#define HMC_ADDR   0x3C

#define HMC_CONF_REG_A 0x00
#define HMC_CONF_REG_B 0x01
#define HMC_MODE_REG   0x02

#define HMC_DATA_X_MSB 0x03
#define HMC_DATA_X_LSB 0x04
#define HMC_DATA_Y_MSB 0x05
#define HMC_DATA_Y_LSB 0x06
#define HMC_DATA_Z_MSB 0x07
#define HMC_DATA_Z_LSB 0x08

#define HMC_STATUS_REG 0x09
#define HMC_ID_REG_A   0x0A
#define HMC_ID_REG_B   0x0B
#define HMC_ID_REG_C   0x0C

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
#define MD_0_CONT  0x00           //Continuous Measurement Mode
#define MD_1_SING  0x01           //Single Measurement Mode (def)

class HMC5883 : public I2C_Base{
	public:
	uint8_t data_rate_reg, gain_reg, avg_samp_reg;
//	uint8_t  data_rate_val;
	float gain_val;

	HMC5883(I2C_TypeDef* I2Cxn, uint8_t average_sample = MA_1, uint8_t data_rate = DO_4, uint8_t gain = GN_1) :I2C_Base(I2Cxn,STANDART){
		if(gain == GN_0){ gain_val = 0.88; gain_reg = gain; data_rate_reg = data_rate; avg_samp_reg = average_sample;}
   else if(gain == GN_1){ gain_val = 1.30; gain_reg = gain; data_rate_reg = data_rate; avg_samp_reg = average_sample;}
   else if(gain == GN_2){ gain_val = 1.90; gain_reg = gain; data_rate_reg = data_rate; avg_samp_reg = average_sample;}
   else if(gain == GN_3){ gain_val = 2.50; gain_reg = gain; data_rate_reg = data_rate; avg_samp_reg = average_sample;}
   else if(gain == GN_4){ gain_val = 4.00; gain_reg = gain; data_rate_reg = data_rate; avg_samp_reg = average_sample;}
   else if(gain == GN_5){ gain_val = 4.70; gain_reg = gain; data_rate_reg = data_rate; avg_samp_reg = average_sample;}
   else if(gain == GN_6){ gain_val = 5.60; gain_reg = gain; data_rate_reg = data_rate; avg_samp_reg = average_sample;}
   else if(gain == GN_7){ gain_val = 8.10; gain_reg = gain; data_rate_reg = data_rate; avg_samp_reg = average_sample;}
	}

	void config();
	void mag_read(uint16_t *data);
	void mag_conv(float &heading_deg);
};
