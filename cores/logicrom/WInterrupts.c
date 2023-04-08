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

static void gpio_common_cb(int handle, int pin, int level)
{
	(void)pin;
	(void)level;
	void (*irq_callback)(void) = gpio_getuserdata(handle);
	if (irq_callback)
		irq_callback();
}

/*
 * \brief Specifies a named Interrupt Service Routine (ISR) to call when an interrupt occurs.
 *        Replaces any previous function that was attached to the interrupt.
 */
void attachInterrupt(pin_size_t pin, voidFuncPtr callback, PinStatus mode)
{
	int handle, trigger = 0;

	handle = io_gethandle(pin);
	if (!handle)
		return;

	switch (mode) {
	case RISING:
		trigger = GPIO_TRIG_HIGH;
		break;
	case FALLING:
		trigger = GPIO_TRIG_LOW;
		break;
	case CHANGE:
		trigger = GPIO_TRIG_BOTH;
		break;
	default:
		return;
	}

	/*
	 * TODO: Check for fast IRQ on 2G modules
	 * Use slow scanning mode for now
	 */
	gpio_trigger_enable(handle, gpio_common_cb, 0, trigger);
	gpio_setuserdata(handle, callback);
}

/*
 * \brief Turns off the given interrupt.
 */
void detachInterrupt(pin_size_t pin)
{
	int handle = io_gethandle(pin);

	if (!handle)
		return;
	gpio_trigger_disable(handle);
	gpio_setuserdata(handle, NULL);
}
