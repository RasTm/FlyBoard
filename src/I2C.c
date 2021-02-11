#include "I2C.h"

void write_byte(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t data){

	I2C1-> CR1 |= 0x0001;			//Peripheral Enable
	I2C1-> CR1 |= 0x0100;			//Start Generation
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	uint16_t Status1 = I2C1-> SR1;
	uint16_t Status2 = I2C1-> SR2;
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2C1-> DR = dev_reg_addr;		//Register Address Write to I2C1_DR Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2C1-> DR = data;				//Register Data Write to I2C1_DR Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2C1-> CR1 |= 0x0200; 			//Stop Generated And Peripheral Disabled
}

void multi_byte_write(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *data, uint8_t byte_count){

	I2C1-> CR1 |= 0x0001;			//Peripheral Enable
	I2C1-> CR1 |= 0x0100;			//Start Generation
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	uint16_t Status1 = I2C1-> SR1;
	uint16_t Status2 = I2C1-> SR2;
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2C1-> DR = dev_reg_addr;		//Register Address Write to I2C1_DR Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(uint8_t i=0; i<byte_count; i++){
		I2C1-> DR = data[i];			//Register Data Write to I2C1_DR Register
		while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
		while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register
	}
	I2C1-> CR1 |= 0x0200;			//Stop Generated And Peripheral Disabled
}

void read_byte(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *data){

	I2C1-> CR1 |= 0x0001;			//Peripheral Enable
	I2C1-> CR1 |= 0x0100;			//Start Generation
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	uint16_t Status1 = I2C1-> SR1;
	uint16_t Status2 = I2C1-> SR2;
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2C1-> DR = dev_reg_addr;		//Register Address Write to I2C1_DR Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2C1-> CR1 |= 0x0100;			//ReStart Generation
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register
	dev_addr |= 1;					//Mpu6050's address value OR operation for I2C Read Sequence
	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	Status1 = I2C1-> SR1;
	Status2 = I2C1-> SR2;
	while (!(I2C1-> SR1 & 0x0040)); //Check Byte Transfer Finish Flag From I2C1_SR1 Register
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	*data = I2C1-> DR; 				//Takes Data From I2C1_DR Register

	I2C1-> CR1 |= 0x0200;			//Stop Generated And Peripheral Disabled
}

void multi_byte_read(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *data, uint8_t byte_count){

	I2C1-> CR1 |= 0x0001;			//Peripheral Enable
	I2C1-> CR1 |= 0x0100;			//Start Generation
	I2C1-> CR1 |= 0x0400;			//Ancknowledge Sending Enable
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	uint16_t Status1 = I2C1-> SR1;
	uint16_t Status2 = I2C1-> SR2;
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2C1-> DR = dev_reg_addr;		//Register Address Write to I2C1_DR Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2C1-> CR1 |= 0x0100;			//ReStart Generation
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register
	dev_addr |= 1;					//Device address value OR operation for I2C Read Sequence
	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	Status1 = I2C1-> SR1;
	Status2 = I2C1-> SR2;
	while (!(I2C1-> SR1 & 0x0040)); //Check Byte Transfer Finish Flag From I2C1_SR1 Register
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(uint8_t i = 0; i < byte_count; i++){
		while(!(I2C1-> SR1 & 0x0040)); //Check RXNE Flag From I2C1_SR1 Register

		if(i == (byte_count-2)){
			I2C1-> CR1 &= 0xFBFF;		//Anknowledge Disabled
		}

		data[i] = I2C1-> DR; 			//Takes Data From I2C1_DR Register
		Status1 = I2C1-> SR1;
		Status2 = I2C1-> SR2;

		if(i == (byte_count-1)){
			I2C1-> CR1 |= 0x0200;		//Stop Generated And Peripheral Disabled
		}
	}
}
/*-------------------------------------FOR SMALL WRITE OPERATION------------------------------------*/
/**************************************USE FOR REGISTERLESS SLAVES***********************************/

void write_byte_small(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t data){

	I2C1-> CR1 |= 0x0001;			//Peripheral Enable
	I2C1-> CR1 |= 0x0100;			//Start Generation
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	uint16_t Status1 = I2C1-> SR1;
	uint16_t Status2 = I2C1-> SR2;
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2C1-> DR = data;				//Register Data Write to I2C1_DR Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2C1-> CR1 |= 0x0200; 			//Stop Generated And Peripheral Disabled
}

void multi_byte_write_small(uint8_t dev_addr, uint8_t *data, uint8_t byte_count){

	I2C1-> CR1 |= 0x0001;			//Peripheral Enable
	I2C1-> CR1 |= 0x0100;			//Start Generation
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	uint16_t Status1 = I2C1-> SR1;
	uint16_t Status2 = I2C1-> SR2;
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(uint8_t i=0; i<byte_count; i++){
		I2C1-> DR = data[i];			//Register Data Write to I2C1_DR Register
		while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
		while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register
	}
	I2C1-> CR1 |= 0x0200;			//Stop Generated And Peripheral Disabled
}

/*-------------------------------------FOR SMALL READ OPERATION-------------------------------------*/
/**************************************USE FOR REGISTERLESS SLAVES***********************************/

void read_byte_small(uint8_t dev_addr, uint8_t *data){

	I2C1-> CR1 |= 0x0001;			//Peripheral Enable
	I2C1-> CR1 |= 0x0100;			//Start Generation
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	dev_addr |= 1;					//Mpu6050's address value OR operation for I2C Read Sequence
	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	int Status1 = I2C1-> SR1;
	int Status2 = I2C1-> SR2;
	while (!(I2C1-> SR1 & 0x0040)); //Check Byte Transfer Finish Flag From I2C1_SR1 Register
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	*data = I2C1-> DR; 				//Takes Data From I2C1_DR Register

	I2C1-> CR1 |= 0x0200;			//Stop Generated And Peripheral Disabled
}

void multi_byte_read_small(uint8_t dev_addr, uint8_t *data, uint8_t byte_count){

	I2C1-> CR1 |= 0x0001;			//Peripheral Enable
	I2C1-> CR1 |= 0x0100;			//Start Generation
	I2C1-> CR1 |= 0x0400;			//Ancknowledge Sending Enable
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	dev_addr |= 1;					//Device address value OR operation for I2C Read Sequence
	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	int Status1 = I2C1-> SR1;
	int Status2 = I2C1-> SR2;
	while (!(I2C1-> SR1 & 0x0040)); //Check Byte Transfer Finish Flag From I2C1_SR1 Register
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(uint8_t i = 0; i < byte_count; i++){
		while(!(I2C1-> SR1 & 0x0040)); //Check RXNE Flag From I2C1_SR1 Register

		if(i == (byte_count-2)){
			I2C1-> CR1 &= 0xFBFF;		//Anknowledge Disabled
		}

		data[i] = I2C1-> DR; 			//Takes Data From I2C1_DR Register
		Status1 = I2C1-> SR1;
		Status2 = I2C1-> SR2;

		if(i == (byte_count-1)){
			I2C1-> CR1 |= 0x0200;		//Stop Generated And Peripheral Disabled
		}
	}
}

/*--------------------------------FOR LARGE ADDRESS WRITE OPERATION---------------------------------*/
/*---------------------------------USE FOR 2 BYTE ADDRESSED SLAVES----------------------------------*/

/*
 * This function for use like eeprom devices etc.
 */

void multi_byte_read_extend(uint8_t dev_addr, uint16_t dev_reg_addr, uint8_t *data, uint8_t byte_count){

	uint8_t dev_reg_addr_divider[2];

	dev_reg_addr_divider[0] = ((dev_reg_addr &= 0x0FFF)>>8);
	dev_reg_addr_divider[1] =  (dev_reg_addr &= 0x00FF);

	I2C1-> CR1 |= 0x0001;			//Peripheral Enable
	I2C1-> CR1 |= 0x0100;			//Start Generation
	I2C1-> CR1 |= 0x0400;			//Ancknowledge Sending Enable
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2C1-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	uint16_t Status1 = I2C1-> SR1;
	uint16_t Status2 = I2C1-> SR2;
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(short int i = 0; i<2; i++){

		I2C1-> DR = dev_reg_addr_divider[i];	//Register Address Write to I2C1_DR Register
		while(!(I2C1-> SR1 & 0x0080));			//Clearing Data Register Flag from I2C1_SR1 Register
		while(I2C1-> SR1 & 0x0400);				//Check Anknowledge Failure From I2C1_SR1 Register
	}

	I2C1-> CR1 |= 0x0100;			//ReStart Generation
	while(!(I2C1-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register
	dev_addr |= 1;					//Device address value OR operation for I2C Read Sequence
	I2C1-> DR = dev_addr;			//Write to I2C1 Data Register
	while(!(I2C1-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	Status1 = I2C1-> SR1;
	Status2 = I2C1-> SR2;
	while (!(I2C1-> SR1 & 0x0040)); //Check Byte Transfer Finish Flag From I2C1_SR1 Register
	while(I2C1-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(uint8_t i = 0; i < byte_count; i++){
		while(!(I2C1-> SR1 & 0x0040));  //Check RXNE Flag From I2C1_SR1 Register

		if(i == (byte_count-2)){
			I2C1-> CR1 &= 0xFBFF;		//Anknowledge Disabled
		}

		data[i] = I2C1-> DR; 			//Takes Data From I2C1_DR Register
		Status1 = I2C1-> SR1;
		Status2 = I2C1-> SR2;

		if(i == (byte_count-1)){
			I2C1-> CR1 |= 0x0200;		//Stop Generated And Peripheral Disabled
		}
	}
}

