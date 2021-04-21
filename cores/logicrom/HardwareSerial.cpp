#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/termios.h>
#include <sys/ioctl.h>
#include <string.h>

#include <variant.h>

#include "HardwareSerial.h"

#define UART_BAUDRATE_CASE(x)		case x: baud = B##x; break;

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
HardwareSerial Serial(UART0);
HardwareSerial Serial1(UART1);
HardwareSerial Serial2(UART2);
#if defined(PLATFORM_S20U) || defined(PLATFORM_M56)
HardwareSerial USBSerial(USBUART);
#endif
HardwareSerial BTSerial(BTSPPHOST);
#endif

void serialEvent() __attribute__((weak));
void serialEvent1() __attribute__((weak));
void serialEvent2() __attribute__((weak));
#if defined(PLATFORM_S20U) || defined(PLATFORM_M56)
void usbSerialEvent() __attribute__((weak));
#endif
void btSerialEvent() __attribute__((weak));

void serialEventRun(void)
{
	if (serialEvent && Serial.available())
		serialEvent();

	if (serialEvent1 && Serial1.available())
		serialEvent1();

	if (serialEvent2 && Serial2.available())
		serialEvent2();

#if defined(PLATFORM_S20U) || defined(PLATFORM_M56)
	if (usbSerialEvent && USBSerial.available())
		usbSerialEvent();
#endif

	if (btSerialEvent && BTSerial.available())
		btSerialEvent();
}

HardwareSerial::HardwareSerial(const char *port_file) : _port_file(port_file), _fd(-1)
{
	is_stdio = !strcmp(_port_file, DEFAULT_STDIO_PORT);
	if (is_stdio)
		_fd = STDOUT_FILENO;
}

void HardwareSerial::begin(unsigned long baud, uint32_t config)
{
	struct termios t;
	int ret = 0;

	if (_port_file == NULL)
		return;

	if (_fd > 0)
		close(_fd);

	/* No need to init fd when stdio */
	if (!is_stdio) {
		_fd = open(_port_file, O_RDWR | O_NONBLOCK);
		if (_fd < 0)
			return;
	}

	switch (baud) {
	default:
		baud = B115200;
		break;
	UART_BAUDRATE_CASE(75)
	UART_BAUDRATE_CASE(150)
	UART_BAUDRATE_CASE(300)
	UART_BAUDRATE_CASE(600)
	UART_BAUDRATE_CASE(1200)
	UART_BAUDRATE_CASE(2400)
	UART_BAUDRATE_CASE(4800)
	UART_BAUDRATE_CASE(9600)
	UART_BAUDRATE_CASE(19200)
	UART_BAUDRATE_CASE(38400)
	UART_BAUDRATE_CASE(57600)
	UART_BAUDRATE_CASE(115200)
	UART_BAUDRATE_CASE(230400)
	UART_BAUDRATE_CASE(460800)
	UART_BAUDRATE_CASE(921600)
	UART_BAUDRATE_CASE(1500000)
	}

	do {
		ret = tcgetattr(_fd, &t_orig);
		if (ret) {
			debug(DBG_OFF, "Fail to get attr for %s: %d\n", _port_file, ret);
			break;
		}

		if (!is_stdio)
			cfmakeraw(&t);
		else
			memcpy(&t, &t_orig, sizeof(struct termios));

		t.c_cflag &= ~(CSIZE | CSTOPB | PARENB | PARODD);
		t.c_cflag |= config;
		cfsetispeed(&t, baud);

		ret = tcsetattr(_fd, TCSANOW, &t);
		if (ret)
			debug(DBG_OFF, "Fail to set attr for %s: %d\n", _port_file, ret);
	} while (0);

	if (ret) {
		close(_fd);
		_fd = 0;
	}
}

void HardwareSerial::begin(unsigned long baud)
{
	begin(baud, SERIAL_8N1);
}

void HardwareSerial::end(void)
{
	tcsetattr(_fd, TCSANOW, &t_orig);

	if (_fd)
		close(_fd);
	_fd = -1;
}

int HardwareSerial::available(void)
{
	unsigned long avail = 0;

	ioctl(_fd, FIONREAD, &avail);

	return avail;
}

int HardwareSerial::availableForWrite(void)
{
	/*
	 * Internal buffer is always big enough for at least 128 bytes
	 */
	return 128;
}

int HardwareSerial::peek(void)
{
	int peek_byte;

	if (ioctl(_fd, TIOCPEEK, &peek_byte) < 0)
		return -1;

	return peek_byte;
}

int HardwareSerial::read(void)
{
	unsigned char c;
	int ret;

	ret = ::read(_fd, &c, 1);
	if (ret <= 0)
		return -1;

	return c;
}

size_t HardwareSerial::write(uint8_t c)
{
	return ::write(_fd, &c, 1);
}

void HardwareSerial::flush(void)
{
	tcdrain(_fd);
}
