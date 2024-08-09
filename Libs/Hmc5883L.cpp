#include "Hmc5883L.hpp"

void HMC5883::config(){
	write_byte(HMC_ADDR, HMC_CONF_REG_A, avg_samp_reg + data_rate_reg);
	write_byte(HMC_ADDR, HMC_CONF_REG_B, gain_reg);
	write_byte(HMC_ADDR, HMC_MODE_REG, MD_0_CONT);
}

void HMC5883::mag_read(uint8_t *data){
	uint8_t pre_data[6] = {0};
	multi_byte_read(HMC_ADDR,HMC_DATA_X_MSB,pre_data,6);
	data[0] = ((pre_data[0]<<8) + pre_data[1]);
	data[1] = ((pre_data[2]<<8) + pre_data[3]);
	data[2] = ((pre_data[4]<<8) + pre_data[5]);
}

