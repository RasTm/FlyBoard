#include "Mpu6050.hpp"

/**
  * @brief  This method allows you to configure MPU6050 through I2C interface.
  * @retval -
  */
void MPU6050::config(){
	write_byte(MPU6050_ADDR0, MPU6050_PWR_MGMT1, 0);						//Mpu6050 outoff the sleep mode
	write_byte(MPU6050_ADDR0, MPU6050_CONFIG, 0x02);                        //Mpu6050 low pass filter set to 3ms
	write_byte(MPU6050_ADDR0, MPU6050_ACCEL_CONF, accel_fs_reg);			//Mpu6050 Accel Set to 2g mode
	write_byte(MPU6050_ADDR0, MPU6050_GYRO_CONF,  gyro_fs_reg);	     		//Mpu6050 Gyro  Set to 250 degre/sec
	delay(30);																//For Gyroscope Startup Time
}

/**
  * @brief  This method allows you to receive 3 axis of accelerometer data.
  * @retval 3 axis of accelerometer data int16 vector format
  */
std::vector<int16_t> MPU6050::get_accel_v(){
	std::vector<uint8_t> pre_data;
	std::vector<int16_t> accel_data;
	multi_byte_read(MPU6050_ADDR0, MPU6050_ACCEL_X_HOUT, pre_data, 6);
	accel_data.insert(accel_data.begin()  ,(pre_data[0]<<8 | pre_data[1]));
	accel_data.insert(accel_data.begin()+1,(pre_data[2]<<8 | pre_data[3]));
	accel_data.insert(accel_data.begin()+2,(pre_data[4]<<8 | pre_data[5]));
	return accel_data;
}

void MPU6050::get_accel(int16_t *accel_data){
	uint8_t pre_data[6] = {};
	multi_byte_read(MPU6050_ADDR0, MPU6050_ACCEL_X_HOUT, pre_data, 6);
	accel_data[0] = (pre_data[0]<<8 | pre_data[1]);
	accel_data[1] = (pre_data[2]<<8 | pre_data[3]);
	accel_data[2] = (pre_data[4]<<8 | pre_data[5]);
}

/**
  * @brief  This method allows you to receive 3 axis of gyroscope data.
  * @retval 3 axis of gyroscope data int16 vector format
  */
std::vector<int16_t> MPU6050::get_gyro_v(){
	std::vector<uint8_t> pre_data;
	std::vector<int16_t> gyro_data;
	multi_byte_read(MPU6050_ADDR0, MPU6050_GYRO_X_HOUT, pre_data, 6);
	gyro_data.insert(gyro_data.begin()  ,(pre_data[0]<<8 | pre_data[1])/gyro_fs_val);
	gyro_data.insert(gyro_data.begin()+1,(pre_data[2]<<8 | pre_data[3])/gyro_fs_val);
	gyro_data.insert(gyro_data.begin()+2,(pre_data[4]<<8 | pre_data[5])/gyro_fs_val);
	return gyro_data;
}

void MPU6050::get_gyro(int16_t *gyro_data){
	uint8_t pre_data[6] = {};
	multi_byte_read(MPU6050_ADDR0, MPU6050_GYRO_X_HOUT, pre_data, 6);
	gyro_data[0] = (pre_data[0]<<8 | pre_data[1]);
	gyro_data[1] = (pre_data[2]<<8 | pre_data[3]);
	gyro_data[2] = (pre_data[4]<<8 | pre_data[5]);
}

/**
  * @brief  This method allows you to receive temperature data.
  * @retval Temperature data int16
  */
int16_t MPU6050::get_temp(){
	int16_t temp_data;
	std::vector<uint8_t> pre_data;
	multi_byte_read(MPU6050_ADDR0, MPU6050_TEMP_HOUT, pre_data, 2);
	temp_data = ((pre_data[0]<<8) + pre_data[1]) / 340 + 37;
	return temp_data/10.0;
}

/**
  * @brief  This method allows you to calculate IMU's axis errors.
  * @retval 5 (XY accelerometer, XYZ gyro) axis of error data in vector float format.
  */
std::vector<int32_t> MPU6050::calc_IMU_error_v(){
	std::vector<uint8_t> pre_data;
	std::vector<int16_t> IMU_data;
	std::vector<int32_t> err;
	int32_t accX_err=0, accY_err=0, gyroX_err=0, gyroY_err=0, gyroZ_err=0;
	uint16_t c=0;

	multi_byte_read(MPU6050_ADDR0, MPU6050_ACCEL_X_HOUT, pre_data, 6);
	IMU_data.insert(IMU_data.begin()  ,(int16_t)(pre_data[0]<<8 | pre_data[1]));
	IMU_data.insert(IMU_data.begin()+1,(int16_t)(pre_data[2]<<8 | pre_data[3]));
	IMU_data.insert(IMU_data.begin()+2,(int16_t)(pre_data[4]<<8 | pre_data[5]));

	accX_err = IMU_data[0];
	accY_err = IMU_data[1];

	err.insert(err.begin()  , accX_err);
	err.insert(err.begin()+1, accY_err);

	while(c<SAMPLE){
		multi_byte_read(MPU6050_ADDR0, MPU6050_GYRO_X_HOUT, pre_data, 6);
		IMU_data.insert(IMU_data.begin()  ,(int16_t)(pre_data[0]<<8 | pre_data[1]));
		IMU_data.insert(IMU_data.begin()+1,(int16_t)(pre_data[2]<<8 | pre_data[3]));
		IMU_data.insert(IMU_data.begin()+2,(int16_t)(pre_data[4]<<8 | pre_data[5]));

		gyroX_err += IMU_data[0];
		gyroY_err += IMU_data[1];
		gyroZ_err += IMU_data[2];
		c++;
		delay(1);
	}

	err.insert(err.begin()+2, gyroX_err/SAMPLE);
	err.insert(err.begin()+3, gyroY_err/SAMPLE);
	err.insert(err.begin()+4, gyroZ_err/SAMPLE);

	return err;
}

void MPU6050::calc_IMU_error(int32_t *err){
	uint8_t pre_data[6] = {};
	int16_t IMU_data[3] = {};
	int32_t gyroX_err=0, gyroY_err=0, gyroZ_err=0;
	uint16_t c=0;

	multi_byte_read(MPU6050_ADDR0, MPU6050_ACCEL_X_HOUT, pre_data, 6);
	IMU_data[0] = (pre_data[0]<<8 | pre_data[1]);
	IMU_data[1] = (pre_data[2]<<8 | pre_data[3]);
	IMU_data[2] = (pre_data[4]<<8 | pre_data[5]);

	err[0] = IMU_data[0];
	err[1] = IMU_data[1];

	while(c<SAMPLE){
		multi_byte_read(MPU6050_ADDR0, MPU6050_GYRO_X_HOUT, pre_data, 6);
		IMU_data[0] = (pre_data[0]<<8 | pre_data[1]);
		IMU_data[1] = (pre_data[2]<<8 | pre_data[3]);
		IMU_data[2] = (pre_data[4]<<8 | pre_data[5]);

		gyroX_err += IMU_data[0];
		gyroY_err += IMU_data[1];
		gyroZ_err += IMU_data[2];
		c++;
		delay(3);
	}

	err[2] = (gyroX_err/SAMPLE);
	err[3] = (gyroY_err/SAMPLE);
	err[4] = (gyroZ_err/SAMPLE);
}
