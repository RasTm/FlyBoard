#ifndef MAIN_H_
#define MAIN_H_

/*
 * All Libraries We Use For FC
 */
#include "stm32f4xx.h"
#include "arm_math.h"
#include "stdlib.h"

#include "../Libs/RCC.hpp"
#include "../Libs/ADC.hpp"
#include "../Libs/DMA.hpp"
#include "../Libs/GPIO.hpp"
#include "../Libs/Nvic.hpp"
#include "../Libs/USART.hpp"
#include "../Libs/Remote.hpp"
#include "../Libs/Ms5611.hpp"
#include "../Libs/Mpu6050.hpp"
#include "../Libs/Hmc5883L.hpp"

/*
 * Some Pre Defined Things
 */
#define PROG_BUFF_SIZE 12
#define MPU6050_FLAG 1
#define MS5611_FLAG  2
#define HMC5883_FLAG 3

#define MPU6050_TICK 2
#define MS5611_TICK  11
#define HMC5883_TICK 10

/*
 * Constant Variables
 */
const uint8_t project_header[]  = "Flyboard Pertinax v1.0\n\r";
const uint8_t motivation[]	=
		"\n\r*********************************************************"
		"\n\r*  In The End, What Separates A Man From A Slave?       *"
		"\n\r*  Money? Power? No.                                    *"
		"\n\r*  A Man Chooses A Slave Obeys                          *"
		"\n\r*                                          <Andrew Ryan>*"
		"\n\r*********************************************************";
const uint8_t clear_disp[] = "\033[2J\033[H";
const uint8_t clear_line[] = "\033[2K\r";

/*
 * Variables
 */
uint8_t program_buffer[PROG_BUFF_SIZE] = {0}, buffer_write_able = true;
volatile uint8_t write_index = 0, read_index = 0;

uint8_t  sayac2=0;
uint32_t mpu_Hz_counter=0, mpu_Hz=0, mpu_tick=0,
		 ms_Hz_counter=0, ms_Hz=0, ms_tick=0,
		 hmc_Hz_counter=0, hmc_Hz=0, hmc_tick=0,
		 program_Hz_counter=0;

//MPU6050 Variables
int16_t raw_gyro[3] = {0}, raw_accel[3] = {0};
uint32_t acc_total_vec=0;
float old_gyro_roll=0.0, old_gyro_pitch=0.0, gyro_pitch=0.0, gyro_roll=0.0, acc_pitch=0.0, acc_roll=0.0;
float final_pitch=0.0, final_roll=0.0;

//MS5611 Variables
double data[2] = {0.0}, altitude = 0.0;

//HMC5883L Variables
float heading_degree = 0.0;

//PPM
PPM remote_ppm;

//ADC
uint32_t adc_avg[2] = {0};
uint32_t local_adc[2] = {0};
float64_t adc[2];
bool adc_read_able = false;

#endif
