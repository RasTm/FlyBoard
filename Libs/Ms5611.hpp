#pragma once
#include "Timers.hpp"
#include "I2C.hpp"
#include <vector>
#include "math.h"

#define MS_ADDR    	0xEE 			//CSB Pin Low
#define MS_ADDR1   	0xEC			//CSB Pin High

#define MS_RESET 		0x1E		//Reset I2C Bus

#define MS_CONV_D1_256	0x40		//Digital Pressure OSR 256
#define MS_CONV_D1_512	0x42		//Digital Pressure OSR 512
#define MS_CONV_D1_1024	0x44		//Digital Pressure OSR 1024
#define MS_CONV_D1_2048	0x46		//Digital Pressure OSR 2048
#define MS_CONV_D1_4096	0x48		//Digital Pressure OSR 4096

#define MS_CONV_D2_256	0x50		//Digital Temperature OSR 256
#define MS_CONV_D2_512	0x52		//Digital Temperature OSR 512
#define MS_CONV_D2_1024	0x54		//Digital Temperature OSR 1024
#define MS_CONV_D2_2048	0x56		//Digital Temperature OSR 2048
#define MS_CONV_D2_4096	0x58		//Digital Temperature OSR 4096

#define MS_ADC_READ		0x00		//ADR Read Command
#define MS_PROM_READ	0xA2		//Coefficient Storage Read

#define P_MIN 10
#define P_MAX 1200
#define T_MIN -40
#define T_MAX 85
#define T_REF 20

class MS5611:public I2C_Base{
	private:
	uint8_t d1_selection,d2_selection;		        //d1 Pressure, d2 Temp
	uint32_t raw_preasure = 0, raw_temp = 0;        //Use in get_raw_data() and calc_absolute_val()
	uint16_t coeff_data[6] = {0};					//Sensor special coefficient variables
	public:
	bool conv_complete = false;
	uint8_t counter = 0;

	MS5611(I2C_TypeDef *I2Cxn, uint8_t ms_d1_select = MS_CONV_D1_4096, uint8_t ms_d2_select = MS_CONV_D2_4096) :I2C_Base(I2Cxn,STANDART){
		d1_selection = ms_d1_select;
		d2_selection = ms_d2_select;
	}

	void get_coefficent_v(std::vector<uint16_t> &coeff_data);
	void calculate_absolute_val_v(std::vector<uint16_t> &coeff, double *return_val, double &alt);

	void get_raw_data();
	void get_coefficent();
	void calculate_absolute_val(double *return_val, double &alt);
	void calculate_absolute_val(double &alt);
};
