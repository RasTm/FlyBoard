#include "Ds1307.h"

void Ds1307_SetTime(uint8_t *time_array){

	uint8_t giden[8];
	for(int i = 0; i<8; i++){
		giden[i] = ((time_array[i] / 10) << 4) + (time_array[i] % 10);
	}
	giden[7] = 0x00;
	multi_byte_write(Ds1307_Addres, 0x00, &giden, 8);
}

void Ds1307_GetTime(uint8_t *output_data){

	uint16_t raw_data[7];
	multi_byte_read_small(Ds1307_Addres, &raw_data, 7);

	raw_data[0] = (((raw_data[0] && 0x7F) >> 4) * 10)+(raw_data[0] && 0x0F);	//Seconds
	raw_data[1] = ((raw_data[1] >> 4) * 10)+(raw_data[1] && 0x0F);				//Minutes
	raw_data[2] = ((raw_data[2] >> 4) * 10)+(raw_data[2] && 0x0F);				//Hours
	raw_data[4] = ((raw_data[4] >> 4) * 10)+(raw_data[4] && 0x0F);				//Date
	raw_data[5] = ((raw_data[5] >> 4) * 10)+(raw_data[5] && 0x0F);				//Month
	raw_data[6] = ((raw_data[6] >> 4) * 10)+(raw_data[6] && 0x0F);				//Year

	for(int i=0; i<8; i++){
		output_data[i] = raw_data[i];
	}
}

void Ds1307_EepromWrite(uint16_t reg_addr, uint8_t *data, uint8_t byte_count){

	uint8_t All_data[byte_count+2];

	All_data[0] = ((reg_addr &= 0x0FFF)>>8);
	All_data[1] =  (reg_addr &= 0x00FF);

	for(int i=0; i < byte_count; i++){
		All_data[i+2] = data[i];
	}

	multi_byte_write_small(Ds1307_Eeprom_Addres, &All_data, (byte_count+2));
}

void Ds1307_EepromRead(uint16_t reg_addr, uint8_t *data, uint8_t byte_count){

	multi_byte_read_extend(Ds1307_Eeprom_Addres, reg_addr, data, byte_count);
}
