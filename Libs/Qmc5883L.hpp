#include <stm32f4xx.h>
#include "I2C.hpp"

//Fake HMC5883L

#define RAD_TO_DEG 57.295779513082320876798154814105
#define DEC_ANGLE (6.0 + (7.0/60.0)) / (57.295779513082320876798154814105)  //For Ankara Declination Angle is +6 degree 7 minutes

#ifndef PI
#define PI					3.14159265358979f
#endif

#define QMC_ADDR   0x1A

#define QMC_DATA_X_LSB 0x00
#define QMC_DATA_X_MSB 0x01
#define QMC_DATA_Y_LSB 0x02
#define QMC_DATA_Y_MSB 0x03
#define QMC_DATA_Z_LSB 0x04
#define QMC_DATA_Z_MSB 0x05

#define QMC_STATUS_REG 0x06

#define QMC_TEMP_LSB   0x07
#define QMC_TEMP_MSB   0x08

#define QMC_CONF_REG_1 0x09
#define QMC_CONF_REG_2 0x0A

#define QMC_SET_RESET  0x0B
//CA1 Register
//MD = Mode Select
#define MD_0_SING  0x00           //Standby (default)
#define MD_1_CONT  0x01           //Continuous Measurement Mode

//DO = Data Output Rate (Hz)
#define DO_0       0x00<<2        //10 Hz
#define DO_1       0x01<<2        //50 Hz
#define DO_2       0x02<<2        //100 Hz
#define DO_3       0x03<<2        //200 Hz

//GN = Gain Control Bits
#define GN_0       0x00<<4        // +- 2 Ga
#define GN_1       0x01<<4        // +- 8 Ga

//MA = Samples Averaged per Measurement Output
#define MA_1       0x00<<6        //512 (default)
#define MA_2       0x01<<6        //256
#define MA_4       0x02<<6        //128
#define MA_8       0x03<<6        //64

//CA2 Register
#define INT_EN      0
#define INT_DIS     1
#define ROLL_PTR_EN 1<<6
#define SOFT_RESET  1<<7

class QMC5883 : public I2C_Base{
	public:
	uint8_t data_reg, gain_val;

	QMC5883(I2C_TypeDef* I2Cxn, uint8_t average_sample = MA_1, uint8_t data_rate = DO_3, uint8_t gain = GN_0) :I2C_Base(I2Cxn,STANDART){
		 if(gain == GN_0){ gain_val = 2; data_reg = gain + data_rate + average_sample;}
    else if(gain == GN_1){ gain_val = 8; data_reg = gain + data_rate + average_sample;}
	}

	void config();
	void mag_read(uint16_t *data);
	void mag_conv(float &heading_deg);
	bool read_able();
};
