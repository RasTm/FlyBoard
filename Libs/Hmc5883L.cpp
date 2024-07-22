#include "Hmc5883L.hpp"

void HMC5883::config(){
	write_byte(HMC_ADDR,CONF_REG_A,0x00);

}
