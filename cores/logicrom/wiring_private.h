/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Modified 7 Feb 2017 by Ajay Bhargav <ajay.bhargav@waybyte.in>
*/

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include "api/Common.h"

#ifdef __cplusplus
extern "C" {
#endif

enum _io_mode {
	IO_MODE_GPIO = 1,
	IO_MODE_ADC = 2,
	IO_MODE_PWM = 3,
};

int io_pinvalid(pin_size_t pin);
void io_sethandle(pin_size_t pin, int handle);
int io_gethandle(pin_size_t pin);
int io_getmode(pin_size_t pin);
void io_setmode(pin_size_t pin, uint32_t mode);
uint8_t io_checkmode(pin_size_t pin, uint32_t mode);

int pin2adc_channel(uint32_t pin);
int pin2pwm_channel(uint32_t pin);

#ifdef __cplusplus
} // extern "C"
#endif
