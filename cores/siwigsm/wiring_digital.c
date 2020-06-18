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
*/

#include "Arduino.h"

void pinMode( uint32_t ulPin, uint32_t ulMode )
{
	int flags;

	/* Not a GPIO */
	if (ulPin >= GPIO_PIN_MAX)
		return;

	switch (ulMode) {
	case INPUT:
		flags = GPIO_FLAG_INPUT;
		break;

	case INPUT_PULLUP:
		flags = GPIO_FLAG_INPUT | GPIO_FLAG_PULLUP;
		break;

	case OUTPUT:
		flags = GPIO_FLAG_OUTPUT;
		break;

	default:
		return;
	}

	if (g_ioHandles[ulPin] == 0)
	{
		/* Fresh IO request */
		g_ioHandles[ulPin] = gpio_request(ulPin, flags);
	}
	else
	{
		/* Just configure gpio */
		gpio_setdir(g_ioHandles[ulPin], flags);
	}
	g_ioModes[ulPin] = (ulMode << 8) | IO_MODE_GPIO;
}

void digitalWrite( uint32_t ulPin, uint32_t ulVal )
{
	int flags, mode;

	/* Not a GPIO */
	if (ulPin >= GPIO_PIN_MAX)
		return;

	mode = (g_ioModes[ulPin] >> 8) & 0xff;

	if ((mode == INPUT) || (mode == INPUT_PULLUP)) {
		flags = GPIO_FLAG_INPUT;
		if (ulVal == HIGH)
			flags |= GPIO_FLAG_PULLUP;
	} else {
		flags = GPIO_FLAG_OUTPUT;
		flags |= (ulVal == HIGH) ? GPIO_FLAG_DEFHIGH : GPIO_FLAG_DEFLOW;
	}

	if (g_ioHandles[ulPin] == 0) {
		/* Fresh IO request */
		g_ioHandles[ulPin] = gpio_request(ulPin, flags);
	} else {
		if (mode == OUTPUT)
			gpio_write(g_ioHandles[ulPin], ulVal);
		else
			gpio_setdir(g_ioHandles[ulPin], flags);
	}
}

int digitalRead( uint32_t ulPin )
{
	/* Not a GPIO */
	if (ulPin >= GPIO_PIN_MAX)
		return LOW;

	if (g_ioHandles[ulPin] == 0) {
		/* Fresh IO request */
		g_ioHandles[ulPin] = gpio_request(ulPin, GPIO_FLAG_INPUT | GPIO_FLAG_PULLUP);
		g_ioModes[ulPin] = (INPUT_PULLUP << 8) | IO_MODE_GPIO;
	}

	if (gpio_read(g_ioHandles[ulPin]))
		return HIGH;

	return LOW;
}

