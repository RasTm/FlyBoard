#include "MS5611.hpp"

/**
  * @brief  This method allows you to receive coefficient data.
  * @param  coeff_data: This variable is where do you want to save coefficient data.
  * @retval
  */
void MS5611::get_coefficent_v(std::vector<uint16_t> &coeff_data){
	write_byte_small(MS_ADDR, MS_RESET);
	uint8_t raw_data[2]={0,0};

	for(uint8_t i=0;i<6;i++){
		raw_data[0]=0,raw_data[1]=0;
		delay(50);
		write_byte_small(MS_ADDR, (MS_PROM_READ+(i*2)));
		multi_byte_read_small(MS_ADDR, raw_data, 2);
		coeff_data.insert(coeff_data.begin()+i, ((raw_data[0]<<8)+raw_data[1]));
	}
}

/**
  * @brief  This method allows you to receive coefficient data.
  * @param  coeff_data: This variable is located in class.
  * @retval
  */
void MS5611::get_coefficent(){
	write_byte_small(MS_ADDR, MS_RESET);
	uint8_t raw_data[2]={0,0};

	for(uint8_t i=0;i<6;i++){
		raw_data[0]=0,raw_data[1]=0;
		delay(50);
		write_byte_small(MS_ADDR, (MS_PROM_READ+(i*2)));
		multi_byte_read_small(MS_ADDR, raw_data, 2);
		coeff_data[i] = ((raw_data[0]<<8)+raw_data[1]);
	}
}
/**
  * @brief  This method allows you to receive raw temperature and pressure data.
  * @param  raw_pressure: This variable is where do you want to save raw pressure data.
  * @param  raw_temp    : This variable is where do you want to save raw temperature data.
  * @retval
  */
void MS5611::get_raw_data(){

	uint8_t Raw_val[3] = {0,0,0};
	if(counter == 0){
		write_byte_small(MS_ADDR, d1_selection);		//Pressure Conversion
		counter++;
		//delay(11);
		goto finish;
	}
	if(counter == 1){
		write_byte_small(MS_ADDR, MS_ADC_READ);
		multi_byte_read_small(MS_ADDR, Raw_val, 3);
		raw_preasure = ((Raw_val[0]<<16)+(Raw_val[1]<<8)+Raw_val[2]);
		Raw_val[0]=0,Raw_val[1]=0,Raw_val[2]=0;

		write_byte_small(MS_ADDR, d2_selection);		//temp Conversion
		counter++;
		//delay(11);
		goto finish;
	}

	if(counter == 2){
		write_byte_small(MS_ADDR, MS_ADC_READ);
		multi_byte_read_small(MS_ADDR, Raw_val, 3);
		raw_temp = ((Raw_val[0]<<16)+(Raw_val[1]<<8)+Raw_val[2]);
		conv_complete = true;
		counter = 0;
	}
	finish:;
}

/**
  * @brief  This method allows you to calculate absolute temperature and pressure data.
  * @param  coeff       : This variable is your coefficient data
  * @param  raw_pressure: This variable is your raw pressure data.
  * @param  raw_temp    : This variable is your raw temperature data.
  * @param  return_val  : This is your absolute temperature and pressure data.
  * @param  alt         : This variable holds your altitude (about sea level)
  * @retval
  */
void MS5611::calculate_absolute_val_v(std::vector<uint16_t> &coeff, double *return_val, double &alt){
	if(conv_complete != true){
		get_raw_data();
	}
	else{
		int32_t dT   = raw_temp-(coeff[4]*256);
		int32_t TEMP = 2000+(((int64_t)dT*coeff[5])/8388608);

		int64_t OFF  = ((int64_t)coeff[1]*65536)+(((int64_t)coeff[3]*dT)/128);
		int64_t SENS = ((int64_t)coeff[0]*32768)+(((int64_t)coeff[2]*dT)/256);
		int32_t P	 = ((((raw_preasure*SENS)/2097152)-OFF)/32768);
		return_val[0] = P/100.0;
		return_val[1] = TEMP/100.0;

	//	alt = ((1.0 - (pow((return_val[0]/1013.25),0.1902949) )) * 44307.69396);
		alt = (((pow((1013.25/return_val[0]),0.1902225603956629256229788852958))-1) * (return_val[1]+273.15)) / 0.0065;
	}
}

void MS5611::calculate_absolute_val(double *return_val, double &alt){
	if(conv_complete != true){
		get_raw_data();
	}

	else{
		int32_t dT   = raw_temp-(coeff_data[4]*256);
		int32_t TEMP = 2000+(((int64_t)dT*coeff_data[5])/8388608);

		int64_t OFF  = ((int64_t)coeff_data[1]*65536)+(((int64_t)coeff_data[3]*dT)/128);
		int64_t SENS = ((int64_t)coeff_data[0]*32768)+(((int64_t)coeff_data[2]*dT)/256);
		int32_t P	 = ((((raw_preasure*SENS)/2097152)-OFF)/32768);
		return_val[0] = P/100.0;
		return_val[1] = TEMP/100.0;

	//	alt = ((1.0 - (pow((return_val[0]/1013.25),0.1902949) )) * 44307.69396);
		alt = (((pow((1013.25/return_val[0]),0.1902225603956629256229788852958))-1) * (return_val[1]+273.15)) / 0.0065;
		conv_complete = false;
	}
}

/*
 * @brief Same method as calculate_absolute_val() but it dont return preasure and temp values
 */
void MS5611::calculate_absolute_val(double &alt){
	if(conv_complete != true){
		get_raw_data();
	}

	else{
		int32_t dT   = raw_temp-(coeff_data[4]*256);
		int32_t TEMP = 2000+(((int64_t)dT*coeff_data[5])/8388608);

		int64_t OFF  = ((int64_t)coeff_data[1]*65536)+(((int64_t)coeff_data[3]*dT)/128);
		int64_t SENS = ((int64_t)coeff_data[0]*32768)+(((int64_t)coeff_data[2]*dT)/256);
		int32_t P	 = ((((raw_preasure*SENS)/2097152)-OFF)/32768);

	//	alt = ((1.0 - (pow((return_val[0]/1013.25),0.1902949) )) * 44307.69396);
		alt = (((pow((1013.25/(P/100.0)),0.1902225603956629256229788852958))-1) * ((TEMP/100.0)+273.15)) / 0.0065;
		conv_complete = false;
	}
}
