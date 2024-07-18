#pragma once
#include "stm32f4xx.h"
#include "GPIO.hpp"
#include <vector>

#define STANDART 0
#define FAST 1

class I2C_Base{
	private:
	uint8_t own_i2c_addr = 0;
	uint16_t trash;

	public:
	I2C_TypeDef* I2Cx;
	I2C_Base(I2C_TypeDef *I2Cxn, uint8_t i2c_mode = STANDART){
		if(I2Cxn == I2C1){
			RCC-> APB1ENR |= 0x00200000;                            //I2C_1 Clock Enable
			I2Cx = ((I2C_TypeDef *) I2C1_BASE);
			Set_Gpio(GPIOB, 6, ALTER, DRAIN, HIGH, UP, I2C1__3);	//SCL
			Set_Gpio(GPIOB, 7, ALTER, DRAIN, HIGH, UP, I2C1__3);	//SDA
		}
		else if(I2Cxn == I2C2){
			RCC-> APB1ENR |= 0x00400000;                            //I2C_2 Clock Enable
			I2Cx = ((I2C_TypeDef *) I2C2_BASE);
			Set_Gpio(GPIOB, 10, ALTER, DRAIN, MED, UP, I2C1__3);	//SCL
			Set_Gpio(GPIOB, 11, ALTER, DRAIN, MED, UP, I2C1__3);	//SDA
		}
		else if(I2Cxn == I2C3){
			RCC-> APB1ENR |= 0x00800000;                            //I2C_3 Clock Enable
			I2Cx = ((I2C_TypeDef *) I2C3_BASE);
			Set_Gpio(GPIOB, 8, ALTER, DRAIN, MED, UP, I2C1__3);	    //SCL
			Set_Gpio(GPIOB, 9, ALTER, DRAIN, MED, UP, I2C1__3);	    //SDA
		}
		else while(1);

		I2Cx-> CR2 |= 42;									//Sonra düzeltilecek APB2_CLK yazılacak
		I2Cx-> OAR1|= own_i2c_addr;
		I2Cx-> OAR1|= 0x4000;

		if(i2c_mode == STANDART){
				I2Cx-> CCR 	 |= 0x00D2;
				I2Cx-> TRISE |= 0x002B;
		}
		else if(i2c_mode==FAST) I2Cx-> CCR |= 0x8000;

		else while(1);
	}

	void write_byte(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t data);
	void multi_byte_write(uint8_t dev_addr, uint8_t dev_reg_addr, std::vector<uint8_t> &data);
	void multi_byte_write(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *data, uint8_t byte_count);

	void read_byte(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *data);
	void multi_byte_read(uint8_t dev_addr, uint8_t dev_reg_addr, std::vector<uint8_t> &data, uint8_t byte_count);
	void multi_byte_read(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *data, uint8_t byte_count);

	void write_byte_small(uint8_t dev_addr, uint8_t data);
	void multi_byte_write_small(uint8_t dev_addr, uint8_t *data, uint8_t byte_count);

	void read_byte_small(uint8_t dev_addr, uint8_t *data);
	void multi_byte_read_small(uint8_t dev_addr, uint8_t *data, uint8_t byte_count);

	void multi_byte_read_extend(uint8_t dev_addr, uint16_t dev_reg_addr, uint8_t *data, uint8_t byte_count);
};
