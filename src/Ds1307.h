#include "stm32f4xx.h"
#include "I2C.h"

#define Ds1307_Addres 				0xD0
#define Ds1307_first_reg_Addres		0x00
#define Ds1307_Control_reg_Addres 	0x07
#define Ds1307_Eeprom_Addres 		0xA0

void Ds1307_SetTime(uint8_t *time_array);
void Ds1307_GetTime(uint8_t *output_data);

void Ds1307_EepromWrite(uint16_t reg_addr, uint8_t *data, uint8_t byte_count);
void Ds1307_EepromRead(uint16_t reg_addr, uint8_t *data,  uint8_t byte_count);
