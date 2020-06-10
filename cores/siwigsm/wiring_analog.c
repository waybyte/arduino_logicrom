/*
 Copyright (c) 2011 Arduino.  All right reserved.

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

 Modified 16 Feb 2017 by Ajay Bhargav <ajay.bhargav@siwi.in>
 */

#include "Arduino.h"

#define VAL_TO_DUTY(x)	((x * 100) / ((1 << _writeResolution) - 1))

static int _readResolution = 10;
static int _writeResolution = 8;

void analogReadResolution(int res) {
	_readResolution = res;
}

void analogWriteResolution(int res) {
	_writeResolution = res;
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to) {
	if (from == to)
		return value;
	if (from > to)
		return value >> (from-to);
	else
		return value << (to-from);
}

eAnalogReference analog_reference = AR_DEFAULT;

void analogReference(eAnalogReference ulMode)
{
	analog_reference = ulMode;
}

uint32_t analogRead(uint32_t ulPin)
{
	uint32_t ulValue;
	int ch;

	ch = pin2adc_channel(ulPin);
	if (ch == -1)
		return LOW;

	if ((ulPin < GPIO_PIN_MAX) && !CHECK_MODE(ulPin, ADC)) {
		if (g_ioHandles[ulPin] > 0) {
			gpio_free(g_ioHandles[ulPin]);
			g_ioHandles[ulPin] = 0;
		}
		adc_config(ch);
		g_ioModes[ulPin] = IO_MODE_ADC;
	}

	ulValue = adc_read(ch);
	ulValue = mapResolution(ulValue, 10, _readResolution);

	return ulValue;
}

void analogOutputInit(void) {
	return;
}

void analogWrite(uint32_t ulPin, uint32_t ulValue)
{
	uint32_t duty;
	int ch;

	ch = pin2pwm_channel(ulPin);

	if (ch < 0) {
		if (ulPin < GPIO_PIN_MAX) {
			/* Default to Digital write if available */
			pinMode(ulPin, OUTPUT);
			ulValue = mapResolution(ulValue, _writeResolution, 8);
			if (ulValue < 128)
				digitalWrite(ulPin, LOW);
			else
				digitalWrite(ulPin, HIGH);
		}
		return;
	}

	/* PWM Channel found */
	if (!CHECK_MODE(ulPin, PWM)) {
		if (g_ioHandles[ulPin] > 0) {
			gpio_free(g_ioHandles[ulPin]);
			g_ioHandles[ulPin] = 0;
		}
		g_ioModes[ulPin] = IO_MODE_PWM;
		pwm_init(ch);
	}

	pwm_stop(ch);
	duty = VAL_TO_DUTY(ulValue);
	pwm_config(ch, PWM_FREQUENCY, duty);
	pwm_start(ch);
}

void analogWriteAdvance(uint32_t ulPin, uint32_t freq, uint32_t ulValue)
{
	uint32_t duty;
	int ch;

	ch = pin2pwm_channel(ulPin);

	if (ch < 0) {
		if (ulPin < GPIO_PIN_MAX) {
			/* Default to Digital write if available */
			pinMode(ulPin, OUTPUT);
			ulValue = mapResolution(ulValue, _writeResolution, 8);
			if (ulValue < 128)
				digitalWrite(ulPin, LOW);
			else
				digitalWrite(ulPin, HIGH);
		}
		return;
	}

	/* PWM Channel found */
	if (!CHECK_MODE(ulPin, PWM)) {
		if (g_ioHandles[ulPin] > 0) {
			gpio_free(g_ioHandles[ulPin]);
			g_ioHandles[ulPin] = 0;
		}
		g_ioModes[ulPin] = IO_MODE_PWM;
		pwm_init(ch);
	}

	pwm_stop(ch);
	duty = VAL_TO_DUTY(ulValue);
	pwm_config(ch, freq, duty);
	pwm_start(ch);
}
