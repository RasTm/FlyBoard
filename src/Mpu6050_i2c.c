#include "Mpu6050_i2c.h"
#include "math.h"

/*params:
 * This function for Write single byte to MPU6050
 * uint8_t mpu_addr = MPU6050 address
 * uint8_t reg_addr = Register Address
 * uint8_t data = Register Data
 */
void mpu6050_write(uint8_t mpu_addr, uint8_t reg_addr, uint8_t data){
	write_byte(mpu_addr, reg_addr, data);
}

/* This function for Read single byte from MPU6050
 * params:
 * uint8_t mpu_addr: Mpu6050 address (without R/W bit)
 * uint8_t reg_addr: Mpu6050 Register address for reading data
 * uint8_t data: Register Data save in this variable
 */
void mpu6050_read(uint8_t mpu_addr, uint8_t reg_addr, uint8_t *data){
	read_byte(mpu_addr, reg_addr, data);
}

/* This function for Read multi byte from MPU6050
 * params:
 * uint8_t mpu_addr: Mpu6050 address (without R/W bit)
 * uint8_t reg_addr: Mpu6050 Register address for reading data
 * uint8_t data: Register Data save in this array
 * uint8_t byte_count: How many bytes transferred
 */
void mpu6050_multi_read(uint8_t mpu_addr, uint8_t reg_addr, uint8_t *data, uint8_t byte_count){
	multi_byte_read(mpu_addr, reg_addr, data, byte_count);
}

/* This function for Write multi byte to MPU6050
 * params:
 * uint8_t mpu_addr: Mpu6050 address (without R/W bit)
 * uint8_t reg_addr: Mpu6050 Register address for writing data
 * uint8_t data: Data transmit from this array
 */
void mpu6050_multi_write(uint8_t mpu_addr, uint8_t reg_addr, uint8_t *data, uint8_t byte_count){
	multi_byte_write(mpu_addr, reg_addr, data, byte_count);
}

/* This function used for Mpu6050 Accelerometer Readings
 * params:
 * int16_t accelX_data: Accelerometer data for X axis
 * int16_t accelY_data: Accelerometer data for Y axis
 * int16_t accelZ_data: Accelerometer data for Z axis
 */
void mpu6050_get_accel(int16_t *accelX_data, int16_t *accelY_data, int16_t *accelZ_data){
	uint8_t data[6];
	mpu6050_multi_read(mpu6050_addr0, mpu6050_accel_x_Hout, *data, 6);
	*accelX_data = ((data[0]<<8) + data[1]) / 16384;						//16384 value used for when accelerometer configuration is 2g
	*accelY_data = ((data[2]<<8) + data[3]) / 16384;
	*accelZ_data = ((data[4]<<8) + data[5]) / 16384;
}

/* This function used for Mpu6050 Gyro Readings
 * params:
 * int16_t gyroX_data: Gyro data for X axis
 * int16_t gyroY_data: Gyro data for Y axis
 * int16_t gyroZ_data: Gyro data for Z axis
 */
void mpu6050_get_gyro(uint16_t *gyroX_data, uint16_t *gyroY_data, uint16_t *gyroZ_data){
	uint8_t data[6];
	mpu6050_multi_read(mpu6050_addr0, mpu6050_gyro_x_Hout, *data, 6);
	*gyroX_data = ((data[0]<<8) + data[1]) / 131;							//131 value used for when gyro configuration is 250/s
	*gyroY_data = ((data[2]<<8) + data[3]) / 131;
	*gyroZ_data = ((data[4]<<9) + data[5]) / 131;
}

/* This function used for Mpu6050 Temperature Readings
 * params:
 * int16_t temp_data: Temperature data
 */
void mpu6050_get_temp(int16_t *temp_data){
	uint8_t data[2];
	mpu6050_multi_read(mpu6050_addr0, mpu6050_temp_Hout, *data, 2);
	*temp_data = ((data[0]<<8) + data[1])/340+37;
}

/* This function used for Mpu6050 Gyro self test
 * params:
 * uint16_t final_gyro_val: Gyro Self test result
 */
void mpu6050_gyro_selftest(uint16_t *final_gyro_val){

	float FTx, FTy, FTz;
	uint16_t STRx, STRy, STRz;
	uint16_t x_val, y_val, z_val;
	uint16_t finalx, finaly, finalz;

	uint8_t gyro_data[6];
	uint16_t gyrox0, gyroy0, gyroz0;				//Without self test stored values
	uint16_t gyrox1, gyroy1, gyroz1;				//With self test stored values

	uint8_t self_data[3];

	mpu6050_multi_read(mpu6050_addr0, mpu6050_gyro_x_Hout, &gyro_data, 6);//All gyro axis output without self test
	gyrox0 = (gyro_data[0]<<8)+gyro_data[1];
	gyroy0 = (gyro_data[2]<<8)+gyro_data[3];
	gyroz0 = (gyro_data[4]<<8)+gyro_data[5];

	mpu6050_write(mpu6050_addr0, mpu6050_gyro_conf, 0xE0);				  //All gyro axis self test enable and 250dps

	mpu6050_multi_read(mpu6050_addr0, mpu6050_gyro_x_Hout, &gyro_data, 6);//All gyro axis output with self test
	gyrox1 = (gyro_data[0]<<8)+gyro_data[1];
	gyroy1 = (gyro_data[2]<<8)+gyro_data[3];
	gyroz1 = (gyro_data[4]<<8)+gyro_data[5];

	mpu6050_multi_read(mpu6050_addr0, mpu6050_x_test, &self_data, 3);	  //Receive all gyro axis self test results

	x_val = self_data[0] && 0x1F;
	y_val = self_data[1] && 0x1F;
	z_val = self_data[2] && 0x1F;

	if(x_val != 0){
		FTx = 25*131*pow(1.046,(x_val-1));
	}
	else{ FTx = 0; }

	if(y_val != 0){
		FTy = -25*131*pow(1.046,(y_val-1));
	}
	else{ FTy = 0; }

	if(z_val != 0){
		FTz = 25*131*pow(1.046,(z_val-1));
	}
	else{ FTz = 0; }

	STRx = gyrox1 - gyrox0;
	STRy = gyroy1 - gyroy0;
	STRz = gyroz1 - gyroz0;

	finalx = (STRx-FTx)/FTx;
	finaly = (STRy-FTy)/FTy;
	finalz = (STRz-FTz)/FTz;

	final_gyro_val[0] = finalx;
	final_gyro_val[1] = finaly;
	final_gyro_val[2] = finalz;
}

/* This function used for Mpu6050 Accelerometer self test
 * params:
 * uint16_t final_accel_val: Accelerometer self test result
 */
void mpu6050_accel_selftest(uint16_t *final_accel_val){

	float FTx, FTy, FTz;
	uint16_t STRx, STRy, STRz;
	uint16_t finalx, finaly, finalz;
	uint8_t x_val, y_val, z_val;

	uint8_t accel_data[6];
	uint16_t accelx0, accely0, accelz0;				//Without self test stored values
	uint16_t accelx1, accely1, accelz1;				//With self test stored values

	uint8_t self_data[4];

	mpu6050_multi_read(mpu6050_addr0, mpu6050_accel_x_Hout, &accel_data, 6);//All accelerometer axis output without self test
	accelx0 = (accel_data[0]<<8)+accel_data[1];
	accely0 = (accel_data[2]<<8)+accel_data[3];
	accelz0 = (accel_data[4]<<8)+accel_data[5];

	mpu6050_write(mpu6050_addr0, mpu6050_accel_conf, 0xF0);				  //All accelerometer axis self test enable and 8g

	mpu6050_multi_read(mpu6050_addr0, mpu6050_gyro_x_Hout, &accel_data, 6);//All accelerometer axis output with self test
	accelx1 = (accel_data[0]<<8)+accel_data[1];
	accely1 = (accel_data[2]<<8)+accel_data[3];
	accelz1 = (accel_data[4]<<8)+accel_data[5];

	mpu6050_multi_read(mpu6050_addr0, mpu6050_x_test, &self_data, 4);	  //Receive all gyro axis self test results

	z_val = ((self_data[2] && 0xE0)>>3) + (self_data[3] && 0x03);
	y_val = ((self_data[1] && 0xE0)>>3) + ((self_data[3]>>2) && 0x03);
	x_val = ((self_data[0] && 0xE0)>>3) + (self_data[3]>>4);

	if(x_val != 0){
		FTx = 4096*0.34*pow(0.92,(x_val-1/pow(2,5)-2))/0.34;
	}
	else{ FTx = 0; }

	if(y_val != 0){
		FTy = 4096*0.34*pow(0.92,(y_val-1/pow(2,5)-2))/0.34;
	}
	else{ FTy = 0; }

	if(z_val != 0){
		FTz = 4096*0.34*pow(0.92,(z_val-1/pow(2,5)-2))/0.34;
	}
	else{ FTz = 0; }

	STRx = accelx1 - accelx0;
	STRy = accely1 - accely0;
	STRz = accelz1 - accelz0;

	finalx = (STRx-FTx)/FTx;
	finaly = (STRy-FTy)/FTy;
	finalz = (STRz-FTz)/FTz;

	final_accel_val[0] = finalx;
	final_accel_val[1] = finaly;
	final_accel_val[2] = finalz;
}
