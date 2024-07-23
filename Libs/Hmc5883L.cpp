#include "Hmc5883L.hpp"

void HMC5883::config(){
	write_byte(HMC_ADDR, HMC_CONF_REG_A, avg_samp_reg + data_rate_reg);
	write_byte(HMC_ADDR, HMC_CONF_REG_B, gain_reg);
	write_byte(HMC_ADDR, HMC_MODE_REG, MD_0_CONT);
}
