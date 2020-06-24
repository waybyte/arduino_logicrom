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

  Copyright (c) 2020 SiWi Embedded Solutions.  All right reserved.
  Modified-By: Ajay Bhargav <ajay.bhargav@siwi.in>
*/

#define ARDUINO_MAIN
#include "Arduino.h"

void variant_init() __attribute__((weak));
void variant_init() {}

void variantEventRun() __attribute__((weak));
void variantEventRun() {}

/**
 * Loop task for running loop
 */
static void arduino_task(void *arg)
{
	setup();

	for (;;)
	{
		loop();
	}
}

/*
 * \brief Main entry point of Arduino application
 */
int main(int argc, char *argv[])
{
	/* Variant specific initialization */
	variant_init();

	/* Start Loop task */
	os_create_task(arduino_task, NULL, FALSE);

	/* Main thread for running and handling events */
	while (1) {
		/* Varient specific events, if any */
		variantEventRun();

		/* Serial Events */
		if (serialEventRun)
			serialEventRun();

		/* sleep is required for RTOS to work properly */
		os_sleep(50);
	}
}
