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
#include "wiring_private.h"

#ifdef __cplusplus
 extern "C" {
#endif

void pinMode( pin_size_t pin, PinMode mode )
{
	int flags, handle;

	/* Not a GPIO */
	if (!io_pinvalid(pin))
		return;

	switch (mode) {
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

	handle = io_gethandle(pin);
	if (!handle) {
		/* Fresh IO request */
		handle = gpio_request(io_pin2gpio(pin), flags);
		io_sethandle(pin, handle);
	} else {
		/* Just configure gpio */
		gpio_setdir(handle, flags);
	}
	if (handle)
		io_setmode(pin, (mode << 8) | IO_MODE_GPIO);
}

void digitalWrite( pin_size_t pin, PinStatus value )
{
	int flags, mode, handle;

	/* Not a GPIO */
	if (!io_pinvalid(pin))
		return;

	mode = (io_getmode(pin) >> 8) & 0xff;

	if ((mode == INPUT) || (mode == INPUT_PULLUP)) {
		flags = GPIO_FLAG_INPUT;
		if (value == HIGH)
			flags |= GPIO_FLAG_PULLUP;
	} else {
		flags = GPIO_FLAG_OUTPUT;
		flags |= (value == HIGH) ? GPIO_FLAG_DEFHIGH : GPIO_FLAG_DEFLOW;
	}

	handle = io_gethandle(pin);
	if (!handle) {
		/* Fresh IO request */
		handle = gpio_request(io_pin2gpio(pin), flags);
		io_sethandle(pin, handle);
	} else {
		if (mode == OUTPUT)
			gpio_write(handle, value);
		else
			gpio_setdir(handle, flags);
	}
}

PinStatus digitalRead( pin_size_t pin )
{
	int handle;

	/* Not a GPIO */
	if (!io_pinvalid(pin))
		return LOW;

	handle = io_gethandle(pin);
	if (!handle) {
		/* Fresh IO request */
		handle = gpio_request(io_pin2gpio(pin), GPIO_FLAG_INPUT | GPIO_FLAG_PULLUP);
		io_sethandle(pin, handle);
		io_setmode(pin, (INPUT_PULLUP << 8) | IO_MODE_GPIO);
	}

	if (gpio_read(handle))
		return HIGH;

	return LOW;
}

#ifdef __cplusplus
}
#endif

