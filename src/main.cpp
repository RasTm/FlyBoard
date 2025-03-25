/*********************************************************
 *  In The End, What Separates A Man From A Slave?       *
 *  Money? Power? No.                                    *
 *  A Man Chooses A Slave Obeys                          *
 *                                          ~Andrew Ryan~*
 *********************************************************/

#include "stm32f4xx.h"
#include "arm_math.h"

#include "../Libs/RCC.hpp"
#include "../Libs/GPIO.hpp"
#include "../Libs/Nvic.hpp"
#include "../Libs/USART.hpp"
#include "../Libs/Mpu6050.hpp"
#include "../Libs/Ms5611.hpp"
#include "../Libs/Hmc5883L.hpp"

void GPIO_init(){
	Set_Gpio(GPIOD,12,OUTPUT,PUSH,MED,NOT,SyS);
	Set_Gpio(GPIOD,13,OUTPUT,PUSH,MED,NOT,SyS);
	Set_Gpio(GPIOD,14,OUTPUT,PUSH,MED,NOT,SyS);
	Set_Gpio(GPIOD,15,OUTPUT,PUSH,MED,NOT,SyS);

	Set_Gpio(GPIOA,0,INPUT,PUSH,LOW,DOWN,SyS);
}
void Program_timer(){
	RCC-> APB1ENR |= 0x00000100;             //Timer14 Clock Enable
	TIM14-> DIER  |= 0x0001;                 //Update Interrupt Enable
	TIM14-> PSC    = 83;                     //Tout = ((PSC+1)*(ARR+1))/Tim_clk)
	TIM14-> ARR    = 999;                    //Tout must be 1ms
	TIM14-> CR1   |= 0x0085;                 //ARPE Enable ,URS and CEN Enable
}
void interrupt_init(){
	Set_Interrupt(TIM8_TRG_COM_TIM14_IRQn,2);
	Set_Interrupt(EXTI0_IRQn,1);
	Set_Ext_Interrupt(0,GPIO_A,RISING);
}

uint8_t sayac=1, sayac2=0;
uint32_t program_int_counter=0, program_last_counter=0, program_Hz_counter=0,
		 mpu_Hz_counter=0, mpu_Hz=0, ms_Hz_counter=0, ms_Hz=0;

bool ms_ready = true, ms_complete = false, mpu_ready = true, mpu_complete = false;

int main(void){
	uint8_t project_header[]  = "Flyboard Pertinax v1.0";
	uint8_t motivation[]	=
			"\n\r*********************************************************"
			"\n\r*  In The End, What Separates A Man From A Slave?       *"
			"\n\r*  Money? Power? No.                                    *"
			"\n\r*  A Man Chooses A Slave Obeys                          *"
			"\n\r                                           <Andrew Ryan>*"
			"\n\r*********************************************************";
	uint8_t clear_disp[] = "\033[2J\033[H";
	uint8_t clear_line[] = "\033[2K\r";

	//MPU6050 Variables
	int32_t acc_total_vec=0;
	int16_t raw_gyro[3] = {0}, raw_accel[3] = {0};
	float old_gyro_roll=0.0, old_gyro_pitch=0.0, gyro_pitch=0.0, gyro_roll=0.0, gyro_constant=0.0, acc_pitch=0.0, acc_roll=0.0;
	float final_pitch=0.0, final_roll=0.0;
	bool set_gyro_angles=false;

	//MS5611 Variables
	double altitude = 0.0;

	Clock_init();
	GPIO_init();
	Program_timer();

	USART_Base Serial(USART_6,115200);
	Serial.USART_Transmit(motivation);
	delay(1000);
	Serial.USART_Transmit(clear_disp);
	Serial.USART_Transmit(project_header);

	MPU6050 mpu(I2C1,MPU6050_FS_SEL1,MPU6050_FS_SEL1);
	mpu.config();
	Serial.USART_Transmit("\n\rCalibrating...");
	mpu.calc_IMU_error();
	gyro_constant = (float)(1.0/250.0/mpu.gyro_fs_val);                  //250 Hz Refresh rate
	Serial.USART_Transmit("\033[2K\rCalibration Done Gyro Constant = ");
	Serial.USART_Transmit_float(gyro_constant,8);
	delay(1000);
	Serial.USART_Transmit(clear_line);

	Serial.USART_Transmit("MS5611 Starting");
	MS5611 ms5611(I2C1);
	ms5611.get_coefficent();
	Serial.USART_Transmit(clear_line);
	Serial.USART_Transmit("Roll\t\tPitch\t\tAltitude\n\r");

	interrupt_init();

	while (1){
		if(mpu_ready == true){
			mpu_Hz_counter++;
			mpu.get_gyro(raw_gyro);
			mpu.get_accel(raw_accel);

			raw_accel[0] -= mpu.imu_calibration_error[0];
			raw_accel[1] -= mpu.imu_calibration_error[1];
			raw_accel[2] -= mpu.imu_calibration_error[2];

			raw_gyro[0] -= mpu.imu_calibration_error[3];
			raw_gyro[1] -= mpu.imu_calibration_error[4];
			raw_gyro[2] -= mpu.imu_calibration_error[5];

			gyro_pitch += raw_gyro[0] * gyro_constant;
			gyro_roll  += raw_gyro[1] * gyro_constant;

			old_gyro_pitch = gyro_pitch;
			old_gyro_roll  = gyro_roll;

			gyro_pitch += old_gyro_roll  * arm_sin_f32(raw_gyro[2] * (gyro_constant * RAD_TO_DEG));
			gyro_roll  -= old_gyro_pitch * arm_sin_f32(raw_gyro[2] * (gyro_constant * RAD_TO_DEG));

			acc_total_vec = sqrt((raw_accel[0]*raw_accel[0])+(raw_accel[1]*raw_accel[1])+(raw_accel[2]*raw_accel[2]));
			acc_pitch = asin((float)raw_accel[1]/(float)acc_total_vec)* RAD_TO_DEG;
			acc_roll  = asin((float)raw_accel[0]/(float)acc_total_vec)*-RAD_TO_DEG;

			if(set_gyro_angles){
				gyro_pitch = gyro_pitch * 0.96 + acc_pitch * 0.04;
				gyro_roll  = gyro_roll  * 0.96 + acc_roll  * 0.04;
			}
			else{
				set_gyro_angles = true;
				gyro_pitch = acc_pitch;
				gyro_roll  = acc_roll;
			}

			final_pitch = gyro_pitch;
			final_roll  = gyro_roll ;

			mpu_complete = true;
			mpu_ready = false;
		}

		if(ms_ready == true){
			ms_Hz_counter++;
			ms5611.calculate_absolute_val(altitude);

			ms_complete = true;
			ms_ready = false;
		}

/*
		Serial.USART_Transmit_float(final_roll,5);
		Serial.USART_Transmit("\t\t");
		Serial.USART_Transmit_float(final_pitch,5);
		Serial.USART_Transmit("\t\t");
		Serial.USART_Transmit_float(altitude,5);
		Serial.USART_Transmit("\n\r");
		sayac2++;
		if(sayac2 == 5){
			sayac2 = 0;
			Serial.USART_Transmit("\033[5A\r\033[0J"); //5 row up, go row beginnig erase everything below
		}*/
	}
}

extern "C" { void TIM8_TRG_COM_TIM14_IRQHandler(void){
	if(TIM14-> SR & 0x0001){
		TIM14-> SR = 0;
		program_int_counter++;
		program_Hz_counter++;

		if(program_int_counter < program_last_counter){
			program_int_counter = program_int_counter + program_last_counter;
		}

		if(mpu_complete == true && program_int_counter > 13){  //12ms For MS5611 Temp/Preasure Conv 13ms for guarantee
			ms_ready = true;
			mpu_ready = false;
			mpu_complete = false;
			program_last_counter = program_int_counter;
		}

		if(ms_complete == true || (program_int_counter - program_last_counter) > 2){
            mpu_ready= true;
            ms_ready = false;
            ms_complete = false;
		}

		if(program_Hz_counter == 1000){
			mpu_Hz = mpu_Hz_counter;
			ms_Hz  = ms_Hz_counter;
			program_Hz_counter = 0;
			mpu_Hz_counter = 0;
			ms_Hz_counter  = 0;
		}

		if(program_int_counter == 14){
			program_int_counter = 0;
		}

		Clr_Interrupt_PD(TIM8_TRG_COM_TIM14_IRQn);
	}
}}

extern "C" { void EXTI0_IRQHandler(void){
	sayac += 1;
	if(sayac == 2){
		Set_Gpio_Pin(GPIOD,15,0);
		Set_Gpio_Pin(GPIOD,14,1);
	}
	else if(sayac == 3){
		Set_Gpio_Pin(GPIOD,14,0);
		Set_Gpio_Pin(GPIOD,13,1);
	}
	else if(sayac == 4){
		Set_Gpio_Pin(GPIOD,13,0);
		Set_Gpio_Pin(GPIOD,12,1);
	}
	else if(sayac == 5){
		Set_Gpio_Pin(GPIOD,12,0);
		Set_Gpio_Pin(GPIOD,15,1);
	}
	else if(sayac == 6){
		sayac = 1;
	}
	else if(sayac == 1){
		Set_Gpio_Pin(GPIOD,15,0);
		Set_Gpio_Pin(GPIOD,14,0);
		Set_Gpio_Pin(GPIOD,13,0);
		Set_Gpio_Pin(GPIOD,12,0);
	}
	Clr_Ext_Interrupt_PD(0);
	Clr_Interrupt_PD(EXTI0_IRQn);
}}
