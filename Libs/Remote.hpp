#pragma once
#include "stm32f4xx.h"

struct PPM{
	bool first_valid_edge  = false;
	bool dead_space_seen   = false;
	uint16_t last_time = 0, current_time = 0, delta = 0;
	int32_t channelX[8] = {0};
	int8_t  i=0;
	uint8_t remote_frame_lost_counter = 0;
};
