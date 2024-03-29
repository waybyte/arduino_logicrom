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

#include "Arduino.h"
#include "wiring_private.h"

static unsigned long pulseInCommon(pin_size_t pin, uint8_t state, unsigned long timeout)
{
	int ioHandle = io_gethandle(pin);
	int stateMask = state ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW;

	unsigned long startMicros = micros();

	// wait for any previous pulse to end
	while (gpio_read(ioHandle) == stateMask) {
		if (micros() - startMicros > timeout)
			return 0;
	}

	// wait for the pulse to start
	while (gpio_read(ioHandle) != stateMask) {
		if (micros() - startMicros > timeout)
			return 0;
	}

	unsigned long start = micros();

	// wait for the pulse to stop
	while (gpio_read(ioHandle) == stateMask) {
		if (micros() - startMicros > timeout)
			return 0;
	}
	return micros() - start;
}

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse. */
uint32_t pulseIn(pin_size_t pin, uint8_t state, uint32_t timeout)
{
	return pulseInCommon(pin, state, timeout);
}

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse. */
uint32_t pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout)
{
	return pulseInCommon(pin, state, timeout);
}
