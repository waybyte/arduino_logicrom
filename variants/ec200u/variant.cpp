/*
  Copyright (c) Waybyte Solutions.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License..

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
   See the GNU Lesser General Public License for more details.
*/

#include "Arduino.h"

#ifdef __cplusplus
extern "C"
{
#endif

int g_ioHandles[GPIO_PIN_MAX];
int g_ioModes[GPIO_PIN_MAX];

static const uint32_t adc_channel_map[3][2] = {
	{GPIO_PIN_MAX, ADC_CH0},
	{GPIO_PIN_MAX + 1, ADC_CH1},
	{GPIO_PIN_MAX + 2, ADC_CH2},
};

int pin2adc_channel(uint32_t pin)
{
	for (int i = 0; i < 3; i++) {
		if (adc_channel_map[i][0] == pin)
			return adc_channel_map[i][1];
	}

	return -1;
}

int pin2pwm_channel(uint32_t pin)
{
	if (pin == GPIO_5)
		return PWM_CH0;
	else if (pin == GPIO_13)
		return PWM_CH1;

	return -1;
}

#ifdef __cplusplus
}
#endif

void variant_init(void)
{
	/* variant init */
}
