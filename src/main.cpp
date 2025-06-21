/*********************************************************
 *  In The End, What Separates A Man From A Slave?       *
 *  Money? Power? No.                                    *
 *  A Man Chooses A Slave Obeys                          *
 *                                          ~Andrew Ryan~*
 *********************************************************/

#include "stm32f4xx.h"
#include "arm_math.h"
#include "stdlib.h"

#include "../Libs/RCC.hpp"
#include "../Libs/ADC.hpp"
#include "../Libs/GPIO.hpp"
#include "../Libs/Nvic.hpp"
#include "../Libs/USART.hpp"
#include "../Libs/Remote.hpp"
#include "../Libs/Ms5611.hpp"
#include "../Libs/Mpu6050.hpp"
#include "../Libs/Hmc5883L.hpp"

#define PROG_BUFF_SIZE 12
#define MPU_FLAG 1
#define MS_FLAG  2
#define HMC_FLAG 3

uint8_t program_buffer[PROG_BUFF_SIZE] = {0}, buffer_write_able = true;
volatile uint8_t write_index = 0, read_index = 0;

uint8_t  sayac2=0;
uint32_t mpu_Hz_counter=0, mpu_Hz=0, mpu_tick=0,
		 ms_Hz_counter=0, ms_Hz=0, ms_tick=0,
		 hmc_Hz_counter=0, hmc_Hz=0, hmc_tick=0,
		 program_Hz_counter=0;

const uint8_t project_header[]  = "Flyboard Pertinax v1.0\n\r";
const uint8_t motivation[]	=
		"\n\r*********************************************************"
		"\n\r*  In The End, What Separates A Man From A Slave?       *"
		"\n\r*  Money? Power? No.                                    *"
		"\n\r*  A Man Chooses A Slave Obeys                          *"
		"\n\r                                           <Andrew Ryan>*"
		"\n\r*********************************************************";
const uint8_t clear_disp[] = "\033[2J\033[H";
const uint8_t clear_line[] = "\033[2K\r";

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
uint16_t adc_avg[2] = {0};
uint16_t local_adc[2] = {0};
float adc[2];
bool adc_read_able = false;

void Program_timer(){
	RCC-> APB1ENR |= 0x00000100;             //Timer14 Clock Enable
	TIM14-> DIER  |= 0x0001;                 //Update Interrupt Enable
	TIM14-> PSC    = 83;                     //Tout = ((PSC+1)*(ARR+1))/Tim_clk) -> Tim_clk = 84Mhz
	TIM14-> ARR    = 999;                    //Tout must be 1ms
	TIM14-> CR1   |= 0x0085;                 //ARPE Enable ,URS and CEN Enable
}
void PPM_read_timer(){
	RCC-> APB1ENR  |= 0x00000080;             //Timer13 Clock Enable
	TIM13-> PSC     = 83;                     //Tout = ((PSC+1)*(ARR+1))/Tim_clk) -> Tim_clk = 84Mhz
	TIM13-> ARR     = 40000;                  //Tout = 40ms For Ppm Failsafe (2 frame)
	TIM13-> DIER   |= 0x0001;                 //Timer13 Updade Interrupt Enable
	TIM13-> CR1    |= 0x0001;                 //Timer13 Counter Start
	gpio_config(GPIOE,11,INPUT,PUSH,LOW,UP,SyS);   //GPIOE 11 Configure
}
void DMA2_init(){
	DMA2_Stream0-> CR |=
			DMA_SxCR_MSIZE_0 |                //16 bit Memory Adress Size
			DMA_SxCR_PSIZE_0 |                //16 bit Perpipheral Adress Size (ADC1-> DR)
			DMA_SxCR_MINC    |                //Memory Increment Mode
			DMA_SxCR_CIRC    |                //Circular Mode
			DMA_SxCR_TCIE;                    //Transfer Complete Interrupt

	DMA2_Stream0-> NDTR = 2;
	DMA2_Stream0-> PAR  = (uint32_t) &ADC1->DR;
	DMA2_Stream0-> M0AR = (uint32_t) &adc_avg[0];

	DMA2_Stream0->CR |= 0x00000001;
}
void interrupt_init(){
	Set_Interrupt(TIM8_TRG_COM_TIM14_IRQn,1);
	Set_Interrupt(TIM8_UP_TIM13_IRQn,2);
	Set_Interrupt(DMA2_Stream0_IRQn,0);
	Set_Interrupt(EXTI15_10_IRQn,0);

	Set_Ext_Interrupt(11,GPIO_E,RISING);
}

int main(void){
	Clock_init();
	Program_timer();
	PPM_read_timer();
	DMA2_init();
	I2C_Base::I2C_recover(I2C1);

	ADC_Base batt_V(ADC_1, 1, 0, 7);                                         //GPIOA 1, first, sample = 7
	ADC_Base batt_A(ADC_1, 0, 1, 7);                                         //GPIOA 0, second, sample = 7
	ADC_Base::ADC_dma_enable(ADC1);
	ADC_Base::ADC_scan_enable(ADC1);
	ADC_Base::ADC_continuous_enable(ADC1);

	USART_Base Serial(USART_1,921600);
	Serial.USART_Transmit(motivation);
	delay(1000);
	Serial.USART_Transmit(clear_disp);
	Serial.USART_Transmit(project_header);

	USART_Base Telemetry(USART_6,9600);

	Serial.USART_Transmit("MPU6050 Starting");
	MPU6050 mpu(I2C1,MPU6050_FS_SEL1,MPU6050_FS_SEL1);
	mpu.config();
	Serial.USART_Transmit("\n\rCalibrating...");
	mpu.calc_IMU_error();
	mpu.gyro_constant = (float)(1.0/250.0/mpu.gyro_fs_val);                  //250 Hz Refresh rate
	Serial.USART_Transmit("\033[2K\rCalibration Done Gyro Constant = ");
	Serial.USART_Transmit_float(mpu.gyro_constant,8);
	delay(500);
	Serial.USART_Transmit(clear_disp);

	Serial.USART_Transmit("MS5611 Starting");
	MS5611 ms5611(I2C1);
	ms5611.get_coefficent();
	Serial.USART_Transmit(clear_line);

//	Serial.USART_Transmit("HMC5883L Starting");
//	HMC5883 hmc(I2C1);
//	hmc.config();
//	delay(50);
	Serial.USART_Transmit(clear_line);
	Serial.USART_Transmit("Roll\tPitch\tAltitude\tPreassure\tTemp\tHeading\n\r");

	interrupt_init();
	ADC_Base::ADC_start(ADC1);

	while (1){
		if(program_buffer[read_index] != 0){read_index++;}
		if(read_index == PROG_BUFF_SIZE){ read_index = 0;}

		if(program_buffer[read_index] == MPU_FLAG){
			mpu_Hz_counter++;
			mpu.get_gyro(raw_gyro);
			mpu.get_accel(raw_accel);

			raw_accel[0] -= mpu.imu_calibration_error[0];
			raw_accel[1] -= mpu.imu_calibration_error[1];
			raw_accel[2] -= mpu.imu_calibration_error[2];

			raw_gyro[0]  -= mpu.imu_calibration_error[3];
			raw_gyro[1]  -= mpu.imu_calibration_error[4];
			raw_gyro[2]  -= mpu.imu_calibration_error[5];

			gyro_pitch += raw_gyro[0] * mpu.gyro_constant;
			gyro_roll  += raw_gyro[1] * mpu.gyro_constant;

			old_gyro_pitch = gyro_pitch;
			old_gyro_roll  = gyro_roll;

			gyro_pitch += old_gyro_roll  * arm_sin_f32((raw_gyro[2] * mpu.gyro_constant)*RAD_TO_DEG);
			gyro_roll  -= old_gyro_pitch * arm_sin_f32((raw_gyro[2] * mpu.gyro_constant)*RAD_TO_DEG);

			acc_total_vec = sqrt((raw_accel[0]*raw_accel[0])+(raw_accel[1]*raw_accel[1])+(raw_accel[2]*raw_accel[2]));
			acc_pitch = asin((float)raw_accel[1]/(float)acc_total_vec)* RAD_TO_DEG;
			acc_roll  = asin((float)raw_accel[0]/(float)acc_total_vec)*-RAD_TO_DEG;

			if(mpu.set_gyro_angles){
				gyro_pitch = (gyro_pitch * 0.8) + (acc_pitch * 0.2);
				gyro_roll  = (gyro_roll  * 0.8) + (acc_roll  * 0.2);
			}
			else{
				mpu.set_gyro_angles = true;
				gyro_pitch = acc_pitch;
				gyro_roll  = acc_roll;
			}
			final_pitch = gyro_pitch;
			final_roll  = gyro_roll ;
		}

		if(program_buffer[read_index] == MS_FLAG){
			ms_Hz_counter++;
			ms5611.calculate_absolute_val(data,altitude);
		}

//	    if(program_buffer[read_index] == HMC_FLAG){
//			hmc_Hz_counter++;
//			hmc.mag_conv(heading_degree);
//		}

		if(adc_read_able){
			if(local_adc[0] > 520){
				adc[0] = ((3.3/4096.0)*(float)(local_adc[0]+50))*10;
			}
			adc[1] = ((3.3/4096.0)*(float)(local_adc[1]))/0.028;
			adc_read_able = false;
		}

		if(read_index % 2 ==0){
			Serial.Transmit(final_roll);
			Serial.USART_Transmit("\t");
			Serial.Transmit(final_pitch);
			Serial.USART_Transmit("\t");
			Serial.Transmit(altitude);
			Serial.USART_Transmit("\t");
			Serial.Transmit(data[0]);
			Serial.USART_Transmit("\t");
			Serial.Transmit(data[1]);
			Serial.USART_Transmit("\t");
			Serial.Transmit(heading_degree);
			Serial.USART_Transmit("\t");
			Serial.Transmit(hmc_Hz);
			Serial.USART_Transmit("\t");
			Serial.Transmit(adc[0]);
			Serial.USART_Transmit("\t");
			Serial.Transmit(adc[1]);
//			Serial.USART_Transmit("\t");
//			Serial.Transmit(remote_ppm.channelX[2]);
//			Serial.USART_Transmit("\t");
//			Serial.Transmit(remote_ppm.channelX[3]);
//			Serial.USART_Transmit("\t");
//			Serial.Transmit(remote_ppm.channelX[4]);
//			Serial.USART_Transmit("\t");
//			Serial.Transmit(remote_ppm.channelX[5]);
//			Serial.USART_Transmit("\t");
//			Serial.Transmit(remote_ppm.channelX[6]);
//			Serial.USART_Transmit("\t");
//			Serial.Transmit(remote_ppm.channelX[7]);
			Serial.USART_Transmit("\n\r");
			sayac2++;

			if(sayac2 == 5){
				sayac2 = 0;
				Serial.USART_Transmit("\033[5A\r\033[0J"); //5 row up, go row begining erase everything below
			}
		}
	}
}

extern "C" { void TIM8_TRG_COM_TIM14_IRQHandler(void){
		if(TIM14-> SR & 0x0001){
		TIM14-> SR = 0;
		mpu_tick++;
		ms_tick++;
		hmc_tick++;
		program_Hz_counter++;

		if(write_index == PROG_BUFF_SIZE-1 && read_index < PROG_BUFF_SIZE){
			write_index = 0;
			buffer_write_able = false;
		}
		else{buffer_write_able = true;}

		if(buffer_write_able == true){
			if(mpu_tick > 2){
				mpu_tick = 0;
				if(write_index < 12){
					program_buffer[write_index] = MPU_FLAG;
					write_index++;
				}
			}
			if(ms_tick > 11){
				ms_tick = 0;
				if(write_index < 12){
					program_buffer[write_index] = MS_FLAG;
					write_index++;
				}
			}
//			if(hmc_tick > 10){
//				hmc_tick = 0;
//				if(write_index < 12){
//					program_buffer[write_index] = HMC_FLAG;
//					write_index++;
//				}
//			}
		}

		if(program_Hz_counter == 1000){
			mpu_Hz = mpu_Hz_counter;
			ms_Hz  = ms_Hz_counter / 4;
			hmc_Hz = hmc_Hz_counter;
			program_Hz_counter = 0;
			mpu_Hz_counter = 0;
			ms_Hz_counter  = 0;
			hmc_Hz_counter = 0;
		}
		Clr_Interrupt_PD(TIM8_TRG_COM_TIM14_IRQn);
	}
}}

extern "C" { void EXTI15_10_IRQHandler(void) {
	remote_ppm.current_time = TIM13-> CNT;

	if(!remote_ppm.first_valid_edge){
		remote_ppm.last_time = remote_ppm.current_time;
		remote_ppm.first_valid_edge = true;
		Clr_Ext_Interrupt_PD(11);
		Clr_Interrupt_PD(EXTI15_10_IRQn);
		return;
	}

    remote_ppm.current_time = TIM13->CNT;
	remote_ppm.delta = remote_ppm.current_time - remote_ppm.last_time;
	remote_ppm.last_time = remote_ppm.current_time;

	if(!remote_ppm.dead_space_seen) {
		if(remote_ppm.delta > 4000){ // Sync boþluðu tespiti
			remote_ppm.dead_space_seen = true;
			remote_ppm.i = 0;
		}
	}
	else{
		if(remote_ppm.i < 8){
			remote_ppm.channelX[remote_ppm.i] = remote_ppm.delta-2;  // - 2 is for calibration
			remote_ppm.i += 1;
		}
		if(remote_ppm.i == 8){
			remote_ppm.dead_space_seen = false;
			remote_ppm.i = 0;
			remote_ppm.remote_frame_lost_counter = 0;
		}
	}

	Clr_Ext_Interrupt_PD(11);
	Clr_Interrupt_PD(EXTI15_10_IRQn);
}}

extern "C" { void TIM8_UP_TIM13_IRQHandler(void){
	if(TIM13-> SR & 0x0001){
		TIM13-> SR = 0;
		remote_ppm.remote_frame_lost_counter++;
		if(remote_ppm.remote_frame_lost_counter == 2){
			remote_ppm.remote_frame_lost_counter = 2;
			remote_ppm.first_valid_edge = false;
			remote_ppm.dead_space_seen  = false;
			remote_ppm.channelX[0] = 0;
			remote_ppm.channelX[1] = 0;
			remote_ppm.channelX[2] = 0;
			remote_ppm.channelX[3] = 0;
			remote_ppm.channelX[4] = 0;
			remote_ppm.channelX[5] = 0;
			remote_ppm.channelX[6] = 0;
			remote_ppm.channelX[7] = 0;
		}
		Clr_Interrupt_PD(TIM8_UP_TIM13_IRQn);
	}
}}

extern "C" { void DMA2_Stream0_IRQHandler(void){
	if(DMA2-> LISR & 0x00000020){
		DMA2-> LIFCR = 0x00000020;
		local_adc[0] = adc_avg[0];
		local_adc[1] = adc_avg[1];
		adc_read_able = true;
		Clr_Interrupt_PD(DMA2_Stream0_IRQn);
	}
}}

