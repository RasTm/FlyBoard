#include "stm32f4xx.h"
#pragma once

#define INPUT 0
#define OUTPUT 1
#define ALTER 2
#define ANALOG 3

#define PUSH 0
#define DRAIN 1

#define LOW 0
#define MED 1
#define HIGH 2
#define V_HIGH 3

#define NOT 0
#define UP 1
#define DOWN 2

#define RISING 0
#define FALLING 1
#define BOTH 2

#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6

enum Alternate_: uint8_t {SyS, TIM1_2, TIM3__5, TIM8__11, I2C1__3, SPI1_2, SPI_3, USART1_3,
                   USART4_6, CAN1_2__TIM12_14, OTG_FS_HS, ETHERNET, FSMC__SDIO, Dcmi, EVENTOUT = 15};

void gpio_config(GPIO_TypeDef *Gpio_Port, uint8_t Pin, uint8_t Mode, uint8_t OType, uint8_t Speed, uint8_t Pupdr, Alternate_ AFR);

void write_gpio_io(GPIO_TypeDef *Gpio_Port, uint8_t Pin, uint8_t set_reset);

uint8_t read_gpio_io(GPIO_TypeDef *Gpio_Port, uint8_t Pin);

void reset_gpio_config(GPIO_TypeDef *Gpio_Port, uint8_t Pin);

void Set_Ext_Interrupt(uint8_t Exti, uint8_t GPIO_Port, uint8_t r_f);

void Clr_Ext_Interrupt_PD(uint8_t Exti);
