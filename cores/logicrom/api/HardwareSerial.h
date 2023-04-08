/*
  Copyright (c) 2016 Arduino LLC.  All right reserved.

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

#pragma once

#include <inttypes.h>
#include "Stream.h"
#include <sys/termios.h>

#include <variant.h>

namespace arduino {

#define UART0		"/dev/ttyS0"
#define UART1		"/dev/ttyS1"
#define UART2		"/dev/ttyS2"
#define USBUART		"/dev/ttyUSB0"
#define BTSPPHOST	"/dev/bthost0"

#define SERIAL_PARITY_EVEN	(PARENB)
#define SERIAL_PARITY_ODD	(PARENB | PARODD)
#define SERIAL_PARITY_NONE	(0)

#define SERIAL_STOP_BIT_1	(0)
#define SERIAL_STOP_BIT_2	(CSTOPB)

#define SERIAL_DATA_5	 	(CS5)
#define SERIAL_DATA_6	 	(CS6)
#define SERIAL_DATA_7	 	(CS7)
#define SERIAL_DATA_8	 	(CS8)
#define SERIAL_DATA_MASK	(CSIZE)

#define SERIAL_5N1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE | SERIAL_DATA_5)
#define SERIAL_6N1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE | SERIAL_DATA_6)
#define SERIAL_7N1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE | SERIAL_DATA_7)
#define SERIAL_8N1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE | SERIAL_DATA_8)
#define SERIAL_5N2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE | SERIAL_DATA_5)
#define SERIAL_6N2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE | SERIAL_DATA_6)
#define SERIAL_7N2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE | SERIAL_DATA_7)
#define SERIAL_8N2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE | SERIAL_DATA_8)
#define SERIAL_5E1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN | SERIAL_DATA_5)
#define SERIAL_6E1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN | SERIAL_DATA_6)
#define SERIAL_7E1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN | SERIAL_DATA_7)
#define SERIAL_8E1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN | SERIAL_DATA_8)
#define SERIAL_5E2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN | SERIAL_DATA_5)
#define SERIAL_6E2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN | SERIAL_DATA_6)
#define SERIAL_7E2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN | SERIAL_DATA_7)
#define SERIAL_8E2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN | SERIAL_DATA_8)
#define SERIAL_5O1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD  | SERIAL_DATA_5)
#define SERIAL_6O1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD  | SERIAL_DATA_6)
#define SERIAL_7O1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD  | SERIAL_DATA_7)
#define SERIAL_8O1	(SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD  | SERIAL_DATA_8)
#define SERIAL_5O2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD  | SERIAL_DATA_5)
#define SERIAL_6O2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD  | SERIAL_DATA_6)
#define SERIAL_7O2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD  | SERIAL_DATA_7)
#define SERIAL_8O2	(SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD  | SERIAL_DATA_8)

class HardwareSerial : public Stream
{
public:
	HardwareSerial(const char *port_file);

	void begin(unsigned long);
	void begin(unsigned long baudrate, uint32_t config);
	void end(void);
	int available(void);
	int availableForWrite(void);
	int peek(void);
	int read(void);
	void flush(void);
	size_t write(uint8_t);
	using Print::write; // pull in write(str) and write(buf, size) from Print
	operator bool() { return true; }
protected:
	const char *_port_file;
	int _fd;
	bool is_stdio;
	struct termios t_orig;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
extern HardwareSerial Serial;
extern HardwareSerial Serial1;
#ifdef HAS_SERIAL_UART2
extern HardwareSerial Serial2;
#endif
#ifdef HAS_USB_SERIAL
extern HardwareSerial USBSerial;
#endif
#ifdef HAS_BT_SERIAL
extern HardwareSerial BTSerial;
#endif
#endif

extern void serialEventRun(void) __attribute__((weak));

}