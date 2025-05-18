#include "Qmc5883L.hpp"
#include "math.h"

void QMC5883::config(){
	write_byte(QMC_ADDR, QMC_SET_RESET, 1);
	write_byte(QMC_ADDR, QMC_CONF_REG_1, data_reg);
	write_byte(QMC_ADDR, QMC_CONF_REG_2, (ROLL_PTR_EN | INT_DIS));
}

void QMC5883::mag_read(uint16_t *data){
	uint8_t pre_data[6] = {0};
	multi_byte_read(QMC_ADDR,QMC_DATA_X_LSB,pre_data,6);
	data[0] = (pre_data[0] + (pre_data[1]<<8));
	data[1] = (pre_data[2] + (pre_data[3]<<8));
	data[2] = (pre_data[4] + (pre_data[5]<<8));
}

void QMC5883::mag_conv(float &heading_deg){
	float axis_mag[3] = {0};
	uint16_t raw_data[3] = {0};
	mag_read(raw_data);
	axis_mag[0] = (float)(raw_data[0]*gain_val);                          // X axis
	axis_mag[1] = (float)(raw_data[1]*gain_val);                          // Y axis
	axis_mag[2] = (float)(raw_data[2]*gain_val);                          // Z axis

	float heading = (atan2f(axis_mag[1],axis_mag[0])) + DEC_ANGLE;

	if(heading < 0) heading += 2*PI;
	if(heading > 2*PI) heading -= 2*PI;

	heading_deg = heading * RAD_TO_DEG;
}

bool QMC5883::read_able(){
	uint8_t status = 0;
	read_byte(QMC_ADDR, QMC_STATUS_REG, &status);
    return (status &= 1);
}
