#pragma once
#include "I2C.hpp"
#include "Timers.hpp"
#include "math.h"

#define RAD_TO_DEG 57.295779513082320876798154814105
#define SAMPLE	1000

//MPU6050 registers
#define	MPU6050_ADDR0			0xD0	//mpu6050 i2c address
#define	MPU6050_ADDR1			0xD2	//mpu6050 i2c address (when	ad0	pin	set	high)

#define	MPU6050_X_TEST			0x0D	//mpu6050 X	axis self test address
#define	MPU6050_Y_TEST			0x0E	//mpu6050 Y axis self test address
#define	MPU6050_Z_TEST			0x0F	//mpu6050 Z	axis self test address

#define	MPU6050_AC_TEST			0x10	//mpu6050 Accelometer self test address lower nibble

#define	MPU6050_SMPL_RATE_DIV	0x19	//mpu6050 Sample rate divider address
#define	MPU6050_CONFIG			0x1A	//mpu6050 Configuration	register address
#define	MPU6050_GYRO_CONF		0x1B	//mpu6050 Gyro Configuration register address
#define	MPU6050_ACCEL_CONF		0x1C	//mpu6050 Accelometer Configuration register address
#define	MPU6050_FIFO_EN			0x23	//mpu6050 Fifo enable register address
#define	MPU6050_I2C_MSTR_CTRL	0x24	//mpu6050 i2c master control register address

#define	MPU6050_I2C_SLV0_ADDR	0x25	//mpu6050 i2c slave	0 register address
#define	MPU6050_I2C_SLV0_REG	0x26	//mpu6050 Slave 0 Register address
#define	MPU6050_I2C_SLV0_CTRL	0x27	//mpu6050 Slave 0 Control register address

#define	MPU6050_I2C_SLV1_ADDR	0x28	//mpu6050 i2c slave	1 register address
#define	MPU6050_I2C_SLV1_REG	0x29	//mpu6050 Slave 1 Register address
#define	MPU6050_I2C_SLV1_CTRL	0x2A	//mpu6050 Slave 1 Control register address

#define	MPU6050_I2C_SLV2_ADDR	0x2B	//mpu6050 i2c slave	2 register address
#define	MPU6050_I2C_SLV2_REG	0x2C	//mpu6050 Slave 2 Register address
#define	MPU6050_I2C_SLV2_CTRL	0x2D	//mpu6050 Slave 2 Control register address

#define	MPU6050_I2C_SLV3_ADDR	0x2E	//mpu6050 i2c slave	3 register address
#define	MPU6050_I2C_SLV3_REG	0x2F	//mpu6050 Slave 3 Register address
#define	MPU6050_I2C_SLV3_CTRL	0x30	//mpu6050 Slave 3 Control register address

#define	MPU6050_I2C_SLV4_ADDR	0x31	//mpu6050 i2c slave 4 register address
#define	MPU6050_I2C_SLV4_REG	0x32	//mpu6050 Slave	4 Register address
#define	MPU6050_I2C_SLV4_DO		0x33	//mpu6050 Slave	4 Data Out register	address
#define	MPU6050_I2C_SLV4_CTRL	0x34	//mpu6050 Slave	4 Control register address
#define	MPU6050_I2C_SLV4_DI		0x35	//mpu6050 Slave	4 Data Input register address

#define	MPU6050_MSTR_STAT		0x36	//mpu6050 i2c Master status	register address

#define	MPU6050_INT_PIN_CFG		0x37	//mpu6050 interrupt	pin	config register	address
#define	MPU6050_INT_PIN_EN		0x38	//mpu6050 interrupt	enable register	address
#define	MPU6050_INT_STAT		0x3A	//mpu6050 interrupt	Status register	address

#define	MPU6050_ACCEL_X_HOUT	0x3B	//mpu6050 accelerometer	x out high register	address
#define	MPU6050_ACCEL_X_LOUT	0x3C	//mpu6050 accelerometer	x out low  register	address

#define	MPU6050_ACCEL_Y_HOUT	0x3D	//mpu6050 accelerometer	y out high register	address
#define	MPU6050_ACCEL_Y_LOUT	0x3E	//mpu6050 accelerometer y out low  register address

#define	MPU6050_ACCEL_Z_HOUT	0x3F	//mpu6050 accelerometer	z out high register	address
#define	MPU6050_ACCEL_Z_LOUT	0x40	//mpu6050 accelerometer	z out low  register address

#define	MPU6050_TEMP_HOUT		0x41	//mpu6050 temperature out high register address
#define	MPU6050_TEMP_LOUT		0x42	//mpu6050 temperature out low  register address

#define	MPU6050_GYRO_X_HOUT		0x43	//mpu6050 gyro x out high register address
#define	MPU6050_GYRO_X_LOUT		0x44	//mpu6050 gyro x out low  register address

#define	MPU6050_GYRO_Y_HOUT		0x45	//mpu6050 gyro y out high register address
#define	MPU6050_GYRO_Y_LOUT		0x46	//mpu6050 gyro y out low  register address

#define	MPU6050_GYRO_Z_HOUT		0x47	//mpu6050 gyro z out high register address
#define	MPU6050_GYRO_Z_LOUT		0x48	//mpu6050 gyro z out low  register address

#define	MPU6050_EXTSENS_00		0x49	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_01		0x4A	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_02		0x4B	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_03		0x4C	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_04		0x4D	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_05		0x4E	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_06		0x4F	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_07		0x50	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_08		0x51	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_09		0x52	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_10		0x53	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_11		0x54	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_12		0x55	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_13		0x56	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_14		0x57	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_15		0x58	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_16		0x59	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_17		0x5A	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_18		0x5B	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_19		0x5C	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_20		0x5D	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_21		0x5E	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_22		0x5F	//mpu6050 external sensor data
#define	MPU6050_EXTSENS_23		0x60	//mpu6050 external sensor data

#define	MPU6050_SLV0_DO			0x63	//mpu6050 Slave	0 data output register address
#define	MPU6050_SLV1_DO			0x64	//mpu6050 Slave	1 data output register address
#define	MPU6050_SLV2_DO			0x65	//mpu6050 Slave	2 data output register address
#define	MPU6050_SLV3_DO			0x66	//mpu6050 Slave	3 data output register address

#define	MPU6050_MSTR_DELAY_CTRL 0X67	//mpu6050 i2c Master Delay Control register address
#define	MPU6050_SIGNAL_PATH_RST 0X68	//mpu6050 signal path reset	register address
#define	MPU6050_USER_CTRL		0x6A	//mpu6050 user control register address

#define	MPU6050_PWR_MGMT1		0x6B	//mpu6050 power management 1 register address
#define	MPU6050_PWR_MGMT2		0x6C	//mpu6050 power management 2 register address

#define	MPU6050_FIFO_COUNT_H	0x72	//mpu6050 fifo count high register address
#define	MPU6050_FIFO_COUNT_L	0x73	//mpu6050 fifo count low  register address
#define	MPU6050_FIFO_RW			0x74	//mpu6050 fifo read	write register address

#define	MPU6050_WHOAMI			0x75	//mpu6050 who am ý register	address

//MPU6050 Register Values
#define MPU6050_FS_SEL0			0
#define MPU6050_FS_SEL1			8
#define MPU6050_FS_SEL2			16
#define MPU6050_FS_SEL3			24

#define GYRO_FS_SEL_250			131.0
#define GYRO_FS_SEL_500			65.5
#define GYRO_FS_SEL_1000		32.8
#define GYRO_FS_SEL_2000		16.4

#define ACCEL_FS_SEL_2g			16384.0
#define ACCEL_FS_SEL_4g			8192.0
#define ACCEL_FS_SEL_8g			4096.0
#define ACCEL_FS_SEL_16g		2048.0

class MPU6050 : public I2C_Base{
	public:
	uint8_t gyro_fs_reg, accel_fs_reg;
	float gyro_fs_val, accel_fs_val;
	int32_t imu_calibration_error[6] = {0};				//MPU6050 error storing variable

	MPU6050(I2C_TypeDef* I2Cxn, uint8_t gyro_fs_select = MPU6050_FS_SEL0, uint8_t accel_fs_select = MPU6050_FS_SEL0) :I2C_Base(I2Cxn,STANDART){

		if	   (gyro_fs_select == MPU6050_FS_SEL0) gyro_fs_val = GYRO_FS_SEL_250;
		else if(gyro_fs_select == MPU6050_FS_SEL1) gyro_fs_val = GYRO_FS_SEL_500;
		else if(gyro_fs_select == MPU6050_FS_SEL2) gyro_fs_val = GYRO_FS_SEL_1000;
		else if(gyro_fs_select == MPU6050_FS_SEL3) gyro_fs_val = GYRO_FS_SEL_2000;

		if	   (accel_fs_select == MPU6050_FS_SEL0) accel_fs_val = ACCEL_FS_SEL_2g;
		else if(accel_fs_select == MPU6050_FS_SEL1) accel_fs_val = ACCEL_FS_SEL_4g;
		else if(accel_fs_select == MPU6050_FS_SEL2) accel_fs_val = ACCEL_FS_SEL_8g;
		else if(accel_fs_select == MPU6050_FS_SEL3) accel_fs_val = ACCEL_FS_SEL_16g;

		gyro_fs_reg = gyro_fs_select;
		accel_fs_reg= accel_fs_select;
	}

	void config();
	std::vector <int32_t> calc_IMU_error_v();
	std::vector <int16_t> get_accel_v();
	std::vector <int16_t> get_gyro_v();

	void calc_IMU_error();
	void get_accel(int16_t *accel_data);
	void get_gyro(int16_t *gyro_data);
	int16_t get_temp();
};

void Mpu6050_gyro_selftest(uint16_t *final_gyro_val);
void Mpu6050_accel_selftest(uint16_t *final_accel_val);
