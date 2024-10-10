#include "Hmc5883L.hpp"
#include "math.h"

void HMC5883::config(){
	write_byte(HMC_ADDR, HMC_CONF_REG_A, avg_samp_reg + data_rate_reg);
	write_byte(HMC_ADDR, HMC_CONF_REG_B, gain_reg);
	write_byte(HMC_ADDR, HMC_MODE_REG, MD_0_CONT);
}

void HMC5883::mag_read(uint16_t *data){
	uint8_t pre_data[6] = {0};
	multi_byte_read(HMC_ADDR,HMC_DATA_X_MSB,pre_data,6);
	data[0] = ((pre_data[0]<<8) + pre_data[1]);
	data[1] = ((pre_data[2]<<8) + pre_data[3]);
	data[2] = ((pre_data[4]<<8) + pre_data[5]);
}

void HMC5883::mag_conv(float &heading_deg){
	float axis_mag[3] = {0};
	uint16_t raw_data[3] = {0};
	mag_read(raw_data);
	axis_mag[0] = (float)(raw_data[0]*gain_val);
	axis_mag[1] = (float)(raw_data[1]*gain_val);
	axis_mag[2] = (float)(raw_data[2]*gain_val);

	float heading = atan2f(axis_mag[1],axis_mag[2]);
	heading += DEC_ANGLE;

	if(heading < 0) heading += 2*PI;
	if(heading > 2*PI) heading -= 2*PI;

	heading_deg = heading * RAD_TO_DEG;
}

