/*
  Copyright (c) 2014 Arduino LLC.  All right reserved.

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
*/

#include "Arduino.h"
#include "wiring_private.h"

#define VAL_TO_DUTY(x)	((x * 100) / ((1 << _writeResolution) - 1))

#ifdef __cplusplus
extern "C" {
#endif

static int _readResolution = 10;
static int _writeResolution = 8;
static eAnalogReference analog_reference = AR_DEFAULT;
void analogReadResolution(int res)
{
  _readResolution = res;
}

void analogWriteResolution(int res)
{
  _writeResolution = res;
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to)
{
  if (from == to) {
    return value;
  }
  if (from > to) {
    return value >> (from-to);
  }
  return value << (to-from);
}

void analogReference(uint8_t mode)
{
	analog_reference = mode;
}

int analogRead(pin_size_t pin)
{
	uint32_t value;
	int ch, handle;

	ch = pin2adc_channel(pin);
	if (ch == -1)
		return LOW;

	handle = io_gethandle(pin);
	if (!io_checkmode(pin, IO_MODE_ADC)) {
		if (handle)
			gpio_free(handle);
		adc_config(ch);
		io_setmode(pin, IO_MODE_ADC);
	}

	value = adc_read(ch);
	value = mapResolution(value, 10, _readResolution);

	return value;
}

void analogWrite(pin_size_t pin, int value)
{
	uint32_t duty;
	int ch;

	ch = pin2pwm_channel(pin);

	if (ch < 0) {
		/* Default to Digital write if available */
		pinMode(pin, OUTPUT);
		value = mapResolution(value, _writeResolution, 8);
		if (value < 128)
			digitalWrite(pin, LOW);
		else
			digitalWrite(pin, HIGH);
		return;
	}

	/* PWM Channel found */
	if (!io_checkmode(pin, IO_MODE_PWM)) {
		gpio_free(io_gethandle(pin));
		io_setmode(pin, IO_MODE_PWM);
		pwm_init(ch);
	}

	pwm_stop(ch);
	duty = VAL_TO_DUTY(value);
	pwm_config(ch, PWM_FREQUENCY, duty);
	pwm_start(ch);
}

void analogWriteAdvance(uint32_t pin, uint32_t freq, uint32_t value)
{
	uint32_t duty;
	int ch;

	ch = pin2pwm_channel(pin);

	if (ch < 0) {
		/* Default to Digital write if available */
		pinMode(pin, OUTPUT);
		value = mapResolution(value, _writeResolution, 8);
		if (value < 128)
			digitalWrite(pin, LOW);
		else
			digitalWrite(pin, HIGH);
		return;
	}

	/* PWM Channel found */
	if (!io_checkmode(pin, IO_MODE_PWM)) {
		gpio_free(io_gethandle(pin));
		io_setmode(pin, IO_MODE_PWM);
		pwm_init(ch);
	}

	pwm_stop(ch);
	duty = VAL_TO_DUTY(value);
	pwm_config(ch, freq, duty);
	pwm_start(ch);
}

#ifdef __cplusplus
}
#endif
