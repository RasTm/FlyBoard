#pragma once
#include "stm32f4xx.h"
#include "GPIO.hpp"
#include <vector>

#define STANDARD 0
#define FAST     1

#define DUTY_0   0
#define DUTY_1   1

class I2C_Base{
	private:
	uint8_t own_i2c_addr = 0;
	uint16_t trash;

	public:
	I2C_TypeDef* I2Cx;
	I2C_Base(I2C_TypeDef *I2Cxn, uint8_t i2c_mode = STANDARD){
		if(I2Cxn == I2C1){
			RCC-> APB1ENR |= 0x00200000;                            //I2C_1 Clock Enable
			I2Cx = ((I2C_TypeDef *) I2C1_BASE);
			gpio_config(GPIOB, 6, ALTER, DRAIN, HIGH, UP, I2C1__3);	//SCL
			gpio_config(GPIOB, 7, ALTER, DRAIN, HIGH, UP, I2C1__3);	//SDA
		}
		else if(I2Cxn == I2C2){
			RCC-> APB1ENR |= 0x00400000;                            //I2C_2 Clock Enable
			I2Cx = ((I2C_TypeDef *) I2C2_BASE);
			gpio_config(GPIOB, 10, ALTER, DRAIN, MED, UP, I2C1__3);	//SCL
			gpio_config(GPIOB, 11, ALTER, DRAIN, MED, UP, I2C1__3);	//SDA
		}
		else if(I2Cxn == I2C3){
			RCC-> APB1ENR |= 0x00800000;                            //I2C_3 Clock Enable
			I2Cx = ((I2C_TypeDef *) I2C3_BASE);
			gpio_config(GPIOB, 8, ALTER, DRAIN, MED, UP, I2C1__3);	//SCL
			gpio_config(GPIOB, 9, ALTER, DRAIN, MED, UP, I2C1__3);	//SDA
		}
		else while(1);

		I2Cx-> CR2 |= 42;									//APB1 Clock Speed
		I2Cx-> OAR1 = own_i2c_addr;
		I2Cx-> OAR1|= 0x4000;

		/* How to calculate I2C CCR, TRISE etc
		 * I2C-CCR register hold F/S bit, Duty bit and CCR field [11:0]
		 * F/S: if its 0 = Sm (Standard mode), if its 1 = Fm (Fast mode)
		 * Duty: Fm mode duty cycle. if its 0 Tlow/Thigh = 2, if its 1 Tlow/Thigh= 16/9 (to reach 400kHz)
		 *
		 * Calculation Example for TRISE:
		 * Formula: TRISE = (Max_Rise_Time / Tpclk1) + 1
		 *
		 * Sm mode:
		 * in Sm mode max allowed SCL rise time is 1000ns
		 * if FREQR = 8 (8MHz) it means Tpclk1 = 125ns (1000/8)
		 * so 1000ns / 125ns = 8 it means TRISE must be 9 (8+1)
		 *
		 * if FREQR = 42(42MHz) it means Tpclk1 = 23.8ns
		 * so 1000ns /23.8ns = 42.01 => 42
		 * it means TRISE must be 43 (42+1)
		 *
		 * Fm mode:
		 * in Fm mode max allowed SCL rise time is 300ns
		 * if FREQR = 42 (42MHz) it means Tpclk1 = 23.8ns (1000/42)
		 * so 300ns / 23.8ns = 12.6 => 12
		 * it means TRISE must be 13 (12+1)
		 *
		 *
		 * Calculation Example for CCR:
		 * in Sm mode to generate 100kHz SCL:
		 * Formula: Thigh = Tlow = CCR * Tpclk1
		 *
		 * if FREQR = 8 (8MHz) Tpclk1 = 125ns
		 * half of the 100kHz period (5000ns) / 125ns = 40 -> convert hex -> 0x28
		 * so CCR must be 0x28
		 *
		 * if FREQR = 42 (42MHz) Tpclk1 = 23.8ns (1/42MHz)
		 * half of the 100kHz period (5000ns) / 23.8ns = 210 -> convert hex -> 0xD2
		 * so CCR must be 0xD2
		 *
		 * Calculation Example for CCR:
		 * in Fm mode to generate 400 kHz SCL:
		 * if FREQR = 42 (42MHz) Tpclk1 = 23.8ns
		 *
		 * Fm mode Duty = 0 (Ratio 2:1):
		 * Formula: F_i2c = F_pclk1 / (3 * CCR)  =>  CCR = F_pclk1 / (3 * F_i2c)
		 * Thigh = CCR * Tpclk1
		 * Tlow  = 2 * CCR * Tpclk1
		 * Total = 3 * CCR * Tpclk1
		 * Example: 42MHz / (3 * 400kHz) = 42000000 / 1200000 = 35 (0x23)
		 * CCR = 35
		 *
		 * Fm mode Duty = 1 (Ratio 16:9):
		 * Formula: F_i2c = F_pclk1 / (25 * CCR) =>  CCR = F_pclk1 / (25 * F_i2c)
		 * Thigh = 9 * CCR * Tpclk1
		 * Tlow  = 16 * CCR * Tpclk1
		 * Total = 25 * CCR * Tpclk1
		 * Example: 42MHz / (25 * 400kHz) = 4.2 => 4
		 * CCR = 4
		 */

		if(i2c_mode == STANDARD){
			I2Cx-> CCR 	 = 210 + STANDARD;
			I2Cx-> TRISE = 43;
		}
		else if(i2c_mode == FAST){
			I2Cx-> CCR   = 35 + FAST + DUTY_0;
//			I2Cx-> CCR   = 4  + (FAST<<15) + (DUTY_1<<14);
			I2Cx-> TRISE = 13;
		}

		else while(1);
	}
	static void I2C_recover(I2C_TypeDef *I2Cxn);
	static void I2C_enable_Event_IRQ(I2C_TypeDef *I2Cxn);
	static void I2C_disable_Event_IRQ(I2C_TypeDef *I2Cxn);
	static void I2C_enable_Error_IRQ(I2C_TypeDef *I2Cxn);
	static void I2C_disable_Error_IRQ(I2C_TypeDef *I2Cxn);

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
