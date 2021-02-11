#include "stm32f4xx.h"

#define bmp_addr	 0xEE	//Bmp180 device address without Read Bit

#define bmp_out_xlsb 0xF8	//This bit field using by 19 bit pressure measurement
#define bmp_out_lsb  0xF7	//This bit field using by temperature and pressure measurement
#define bmp_out_msb  0xF6	//This bit field using by temperature and pressure measurement

#define bmp_config   0xF4	//This bit field used for configure Bmp180

#define bmp_soft_rst 0xE0	//This bit field used for software reset to Bmp180 write to 0xB6

#define bmp_id		 0xD0	//This bit field contains Bmp180 id

#define bmp_calib    0xAA	//This bit field going until address reach 0xBF and contains calibration value

#define bmp_config_temp 0x2E
#define bmp_config_pres0 0x34
#define bmp_config_pres1 0x74
#define bmp_config_pres2 0xB4
#define bmp_config_pres3 0xF4

void bmp_get_calib();
void bmp_calc(uint8_t oss, long *temp_data, long *pressure_data, long *altitude);


