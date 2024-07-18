#include "LCD_i2c.hpp"

#define LCD_ADDR 0x4E		//Generic PCF8574 I2C LCD Board Address

/**
  * @brief  This method allows you to send commands to LCD through I2C interface.
  * @param  cmd: This variable holds the command to be sent to the LCD.
  * @retval -
  */
void LCD::lcd_send_cmd(char cmd){
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	multi_byte_write_small(LCD_ADDR,data_t,4);
}

/**
  * @brief  This method allows you to send data to LCD through I2C interface.
  * @param  data: This variable holds the data to be sent to the LCD.
  * @retval -
  */
void LCD::lcd_send_data(char data){
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	multi_byte_write_small(LCD_ADDR,data_t,4);
}

/**
  * @brief  This method allows you to clear LCD screen.
  * @retval -
  */
void LCD::lcd_clear(){
/*	lcd_send_cmd (0x80);
	for (int i=0; i<32; i++){
		lcd_send_data(' ');
	}*/
	lcd_send_cmd(0x01);   // clear display
	delay(2);
}

/**
  * @brief  This method allows you to adjust cursor position of LCD screen.
  * @param  row: This variable holds the row data of cursor.
  * @param  col: This variable holds the column data of cursor.
  * @retval -
  */
void LCD::lcd_put_cur(uint8_t row, uint8_t col){
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }
    lcd_send_cmd(col);
}

/**
  * @brief  This method ensures that the LCD is ready for use.
  * @retval -
  */
void LCD::lcd_init(){
	// 4 bit initialization
	delay(50);            // wait for >40ms
	lcd_send_cmd(0x30);
	delay(5);             // wait for >4.1ms
	lcd_send_cmd(0x30);
	delay(1);             // wait for >100us
	lcd_send_cmd(0x30);
	delay(10);
	lcd_send_cmd(0x20);   // 4bit mode
	delay(10);

	// Display initialization
	lcd_send_cmd(0x28);   // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	delay(1);
	lcd_send_cmd(0x08);   // Display on/off control --> D=0,C=0, B=0  ---> display off
	delay(1);
	lcd_send_cmd(0x01);   // clear display
	delay(1);
	delay(1);
	lcd_send_cmd(0x06);   // Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	delay(1);
	lcd_send_cmd(0x0C);   // Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

/**
  * @brief  This method allows you to send string data to LCD through I2C interface.
  * @param  str: This pointer holds the char data to be sent to the LCD.
  * @retval -
  */
void LCD::lcd_send_string(char *str){
	while (*str) lcd_send_data(*str++);
}

void LCD::lcd_send_int(int data, uint8_t length){
	char string_num[length];
	snprintf(string_num, length+1, "%d", data);

	lcd_send_string(string_num);
}

void LCD::lcd_send_float(double data, uint8_t length){
	char string_num[length];
	snprintf(string_num, length+1, "%f", data);

	lcd_send_string(string_num);
}
