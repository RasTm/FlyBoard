#include "stm32f4xx.h"

void Clock_init();
void RTC_BKP_Write(uint8_t* text, uint8_t n);
void RTC_BKP_Read(uint8_t* data);
