#include "stm32f4xx.h"
#include "I2C.h"

//MPU6050 registers
#define mpu6050_addr0 	      	0xD0	//mpu6050 i2c address
#define mpu6050_addr1			0xD2	//mpu6050 i2c address (when ad0 pin set high)

#define mpu6050_x_test 			0x0D	//mpu6050 X axis self test address
#define mpu6050_y_test			0x0E	//mpu6050 Y axis self test address
#define mpu6050_z_test			0x0F	//mpu6050 Z axis self test address

#define mpu6050_Ac_test			0x10	//mpu6050 Accelometer self test address lower nibble

#define mpu6050_smpl_rate_div	0x19	//mpu6050 Sample rate divider address
#define mpu6050_config			0x1A	//mpu6050 Configuration register address
#define mpu6050_gyro_conf		0x1B	//mpu6050 Gyro Configuration register address
#define mpu6050_accel_conf		0x1C	//mpu6050 Accelometer Configuration register address
#define mpu6050_fifo_en			0x23	//mpu6050 Fifo enable register address
#define mpu6050_i2c_mstr_ctrl	0x24	//mpu6050 i2c master control register address

#define mpu6050_i2c_slv0_addr	0x25	//mpu6050 i2c slave 0 register address
#define mpu6050_i2c_slv0_reg  	0x26	//mpu6050 Slave 0 Register address
#define mpu6050_i2c_slv0_ctrl	0x27	//mpu6050 Slave 0 Control register address

#define mpu6050_i2c_slv1_addr	0x28	//mpu6050 i2c slave 1 register address
#define mpu6050_i2c_slv1_reg  	0x29	//mpu6050 Slave 1 Register address
#define mpu6050_i2c_slv1_ctrl	0x2A	//mpu6050 Slave 1 Control register address

#define mpu6050_i2c_slv2_addr	0x2B	//mpu6050 i2c slave 2 register address
#define mpu6050_i2c_slv2_reg  	0x2C	//mpu6050 Slave 2 Register address
#define mpu6050_i2c_slv2_ctrl	0x2D	//mpu6050 Slave 2 Control register address

#define mpu6050_i2c_slv3_addr	0x2E	//mpu6050 i2c slave 3 register address
#define mpu6050_i2c_slv3_reg  	0x2F	//mpu6050 Slave 3 Register address
#define mpu6050_i2c_slv3_ctrl	0x30	//mpu6050 Slave 3 Control register address

#define mpu6050_i2c_slv4_addr	0x31	//mpu6050 i2c slave 4 register address
#define mpu6050_i2c_slv4_reg  	0x32	//mpu6050 Slave 4 Register address
#define mpu6050_i2c_slv4_DO		0x33	//mpu6050 Slave 4 Data Out register address
#define mpu6050_i2c_slv4_ctrl	0x34	//mpu6050 Slave 4 Control register address
#define mpu6050_i2c_slv4_DI		0x35	//mpu6050 Slave 4 Data Input register address

#define mpu6050_mstr_stat		0x36	//mpu6050 i2c Master status register address

#define mpu6050_INT_pin_cfg		0x37	//mpu6050 interrupt pin config register address
#define mpu6050_INT_pin_en		0x38	//mpu6050 interrupt enable register address
#define mpu6050_INT_stat		0x3A	//mpu6050 interrupt Status register address

#define mpu6050_accel_x_Hout	0x3B	//mpu6050 accelerometer x out high register address
#define mpu6050_accel_x_Lout	0x3C	//mpu6050 accelerometer x out low register address

#define mpu6050_accel_y_Hout	0x3D	//mpu6050 accelerometer y out high register address
#define mpu6050_accel_y_Lout	0x3E	//mpu6050 accelerometer y out low register address

#define mpu6050_accel_z_Hout	0x3F	//mpu6050 accelerometer z out high register address
#define mpu6050_accel_z_Lout	0x40	//mpu6050 accelerometer z out low register address

#define mpu6050_temp_Hout		0x41	//mpu6050 temperature out high register address
#define mpu6050_temp_Lout		0x42	//mpu6050 temperature out low register address

#define mpu6050_gyro_x_Hout		0x43	//mpu6050 gyro x out high register address
#define mpu6050_gyro_x_Lout		0x44	//mpu6050 gyro x out low register address

#define mpu6050_gyro_y_Hout		0x45	//mpu6050 gyro y out high register address
#define mpu6050_gyro_y_Lout		0x46	//mpu6050 gyro y out low register address

#define mpu6050_gyro_z_Hout		0x47	//mpu6050 gyro z out high register address
#define mpu6050_gyro_z_Lout		0x48	//mpu6050 gyro z out low register address

#define mpu6050_extsens_00		0x49	//mpu6050 external sensor data
#define mpu6050_extsens_01		0x4A	//mpu6050 external sensor data
#define mpu6050_extsens_02		0x4B	//mpu6050 external sensor data
#define mpu6050_extsens_03		0x4C	//mpu6050 external sensor data
#define mpu6050_extsens_04		0x4D	//mpu6050 external sensor data
#define mpu6050_extsens_05		0x4E	//mpu6050 external sensor data
#define mpu6050_extsens_06		0x4F	//mpu6050 external sensor data
#define mpu6050_extsens_07		0x50	//mpu6050 external sensor data
#define mpu6050_extsens_08		0x51	//mpu6050 external sensor data
#define mpu6050_extsens_09		0x52	//mpu6050 external sensor data
#define mpu6050_extsens_10		0x53	//mpu6050 external sensor data
#define mpu6050_extsens_11		0x54	//mpu6050 external sensor data
#define mpu6050_extsens_12		0x55	//mpu6050 external sensor data
#define mpu6050_extsens_13		0x56	//mpu6050 external sensor data
#define mpu6050_extsens_14		0x57	//mpu6050 external sensor data
#define mpu6050_extsens_15		0x58	//mpu6050 external sensor data
#define mpu6050_extsens_16		0x59	//mpu6050 external sensor data
#define mpu6050_extsens_17		0x5A	//mpu6050 external sensor data
#define mpu6050_extsens_18		0x5B	//mpu6050 external sensor data
#define mpu6050_extsens_19		0x5C	//mpu6050 external sensor data
#define mpu6050_extsens_20		0x5D	//mpu6050 external sensor data
#define mpu6050_extsens_21		0x5E	//mpu6050 external sensor data
#define mpu6050_extsens_22		0x5F	//mpu6050 external sensor data
#define mpu6050_extsens_23		0x60	//mpu6050 external sensor data

#define mpu6050_slv0_DO			0x63	//mpu6050 Slave 0 data output register address
#define mpu6050_slv1_DO			0x64	//mpu6050 Slave 1 data output register address
#define mpu6050_slv2_DO			0x65	//mpu6050 Slave 2 data output register address
#define mpu6050_slv3_DO			0x66	//mpu6050 Slave 3 data output register address

#define mpu6050_mstr_delay_ctrl 0x67	//mpu6050 i2c Master Delay Control register address
#define mpu6050_signal_path_rst 0x68	//mpu6050 signal path reset register address
#define mpu6050_user_ctrl		0x6A	//mpu6050 user control register address

#define mpu6050_pwr_mgmt1		0x6B	//mpu6050 power management 1 register address
#define mpu6050_pwr_mgmt2		0x6C	//mpu6050 power management 2 register address

#define mpu6050_fifo_count_H	0x72	//mpu6050 fifo count high register address
#define mpu6050_fifo_count_L	0x73	//mpu6050 fifo count low register address
#define mpu6050_fifo_rw			0x74	//mpu6050 fifo read write register address

#define mpu6050_whoami 		  	0x75	//mpu6050 who am ı register address

// Low level functions
void mpu6050_write(uint8_t mpu_addr, uint8_t reg_addr, uint8_t data);
void mpu6050_read(uint8_t mpu_addr, uint8_t reg_addr, uint8_t *data);

void mpu6050_multi_write(uint8_t mpu6050_addr, uint8_t reg_addr, uint8_t *data, uint8_t byte_count);
void mpu6050_multi_read(uint8_t mpu_addr, uint8_t reg_addr, uint8_t *data, uint8_t byte_count);

// Medium level functions
void mpu6050_get_accel(int16_t *accelX_data, int16_t *accelY_data, int16_t *accelZ_data);
void mpu6050_get_gyro(uint16_t *gyroX_data, uint16_t *gyroY_data, uint16_t *gyroZ_data);
void mpu6050_get_temp(int16_t *temp_data);

void mpu6050_gyro_selftest(uint16_t *final_gyro_val);
void mpu6050_accel_selftest(uint16_t *final_accel_val);

