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
void attachInterrupt(uint32_t pin, voidFuncPtr callback, uint32_t mode)
{
	int trigger = 0;

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

	/* TODO: Check for fast IRQ */
	/* Use slow scanning mode for now */
	gpio_settrigger(g_ioHandles[pin], gpio_common_cb, 0, trigger);
	gpio_setuserdata(g_ioHandles[pin], callback);
}

/*
 * \brief Turns off the given interrupt.
 */
void detachInterrupt(uint32_t pin)
{
	gpio_trigger_disable(g_ioHandles[pin]);
	gpio_setuserdata(g_ioHandles[pin], NULL);
}
