#pragma once
#include "stm32f4xx.h"

struct PPM{
	bool first_rising_edge = false;
	bool dead_space_seen   = false;
	int16_t period = 0;
	int16_t channelX[8] = {0};
	int8_t  i=0;
};
