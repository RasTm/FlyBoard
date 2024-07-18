#pragma once
#include "I2C.hpp"
#include "Timers.hpp"

class LCD : public I2C_Base{
	public:
	LCD(I2C_TypeDef *I2Cxn):I2C_Base(I2Cxn, STANDART){}

	void lcd_init();
	void lcd_send_cmd(char cmd);
	void lcd_send_data(char data);
	void lcd_clear();
	void lcd_put_cur(uint8_t row, uint8_t col);
	void lcd_send_string(char *str);
	void lcd_send_int(int data, uint8_t length);
	void lcd_send_float(double data, uint8_t length);
};
