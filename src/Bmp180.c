#include "Bmp180.h"
#include "math.h"

	int16_t AC1 = 0;
	int16_t AC2 = 0;
	int16_t AC3 = 0;
	uint16_t AC4 = 0;
	uint16_t AC5 = 0;
	uint16_t AC6 = 0;
	int16_t B1 = 0;
	int16_t B2 = 0;
	int16_t MB = 0;
	int16_t MC = 0;
	int16_t MD = 0;

	long int UT = 0;
	long int UP = 0;
	long int X1 = 0;
	long int X2 = 0;
	long int X3 = 0;
	long int B3 = 0;
	long int B5 = 0;
	unsigned long int B4 = 0;
	long int B6 = 0;
	unsigned long int B7 = 0;


void bmp_get_calib(){

	uint8_t calib_val[22];

	multi_byte_read(bmp_addr, bmp_calib, &calib_val, 22);

	AC1  = (calib_val[0]<<8)  + calib_val[1];
	AC2  = (calib_val[2]<<8)  + calib_val[3];
	AC3  = (calib_val[4]<<8)  + calib_val[5];
	AC4  = (calib_val[6]<<8)  + calib_val[7];
	AC5  = (calib_val[8]<<8)  + calib_val[9];
	AC6  = (calib_val[10]<<8) + calib_val[11];
	B1   = (calib_val[12]<<8) + calib_val[13];
	B2   = (calib_val[14]<<8) + calib_val[15];
	MB   = (calib_val[16]<<8) + calib_val[17];
	MC   = (calib_val[18]<<8) + calib_val[19];
	MD   = (calib_val[20]<<8) + calib_val[21];
}

void bmp_calc(uint8_t oss, long *temp_data, long *pressure_data, long *altitude){

	uint8_t raw_temp_data[2] = {0};									//raw temp data
	uint8_t raw_pressure_data[3] = {0};								//raw pressure data
	long int pre_pressure_data = 0;
	float sea_level_press = 1013.25;

	write_byte(bmp_addr, bmp_config, bmp_config_temp);				//setup reading for temp
	delay_us(10);													//wait 4.5ms

	multi_byte_read(bmp_addr, bmp_out_msb, &raw_temp_data, 2);		//reading temp values
	UT = ((raw_temp_data[0]<<8) + raw_temp_data[1]); 				//converting UT val

	switch (oss) {													//switch case depended by over sampling settings
		case 0:
			write_byte(bmp_addr, bmp_config, bmp_config_pres0);		//ultra low power mode and setup for pressure
			delay_us(10);
			break;
		case 1:
			write_byte(bmp_addr, bmp_config, bmp_config_pres1);		//standard mode and setup for pressure
			delay_us(15);
			break;
		case 2:
			write_byte(bmp_addr, bmp_config, bmp_config_pres2);		//high resolution mode and setup for pressure
			delay_us(20);
			break;
		case 3:
			write_byte(bmp_addr, bmp_config, bmp_config_pres3);		//ultra high resolution mode and setup for pressure
			delay_us(30);
			break;
		default:
			while(1);
			break;
	}

	multi_byte_read(bmp_addr, bmp_out_msb, &raw_pressure_data, 3);	//reading raw pressure values
	UP = ((raw_pressure_data[0]<<16) + (raw_pressure_data[1]<<8) + (raw_pressure_data[2])) >> (8-oss);	//converting raw to UP val

	X1 = ((UT-AC6) * (AC5/(pow(2,15))));										//
	X2 = ((MC * (pow(2,11))) / (X1 + MD));										//		All 4 lines for calculating true temperature value
	B5 = X1 + X2;																//
	*temp_data = (B5 + 8) / (pow(2,4));											//

	B6 = B5 - 4000;																//
	X1 = (B2 * (B6 * B6 / (pow(2,12)))) / (pow(2,11)); 							//
	X2 = AC2 * B6 / (pow(2,11));												//
	X3 = X1 + X2;																//
    B3 = (((AC1 * 4 + X3) << oss) + 2) / 4;										//
	X1 = AC3 * B6 / pow(2,13);													//
	X2 = (B1 * (B6 * B6 / (pow(2,12)))) / (pow(2,16));							//
	X3 = ((X1 + X2) + 2) / pow(2,2);											//
	B4 = AC4 * (unsigned long)(X3 + 32768) / (pow(2,15));						//
	B7 = ((unsigned long) UP - B3) * (50000 >> oss);							//		All those lines for calculating true pressure value
	if(B7 < 0x80000000){														//
		pre_pressure_data = (B7 * 2) / B4;										//
	}																			//
	else{																		//
		pre_pressure_data = (B7 / B4) * 2;										//
	}																			//
	X1 = (pre_pressure_data / (pow(2,8))) * (pre_pressure_data / (pow(2,8)));	//
	X1 = (X1 * 3038) / (pow(2,16));												//
	X2 = (-7357 * pre_pressure_data) / (pow(2,16));								//
	pre_pressure_data = pre_pressure_data + (X1 + X2 + 3791) / (pow(2,4));		//

	*pressure_data = pre_pressure_data;											//pressure in Pa

	*altitude = 44330 * (1-pow(((pre_pressure_data/100) / sea_level_press),1 / 5.255));

}
