#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/termios.h>
#include <sys/ioctl.h>

#include <lib.h>

#include "HardwareSerial.h"

#define UART_BAUDRATE_CASE(x)		case x: baud = B##x; break;

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
HardwareSerial Serial(UART0);
HardwareSerial Serial1(UART1);
HardwareSerial Serial2(UART2);
HardwareSerial USBSerial(USBUART);
HardwareSerial BTSerial(BTSPPHOST);
#endif

HardwareSerial::HardwareSerial(const char *port_file) : _port_file(port_file), _fd(0) {}

extern "C" size_t read_byte(int fd, unsigned char *c)
{
	return read(fd, c, 1);
}

extern "C" size_t write_byte(int fd, unsigned char *c)
{
	return write(fd, c, 1);
}

void HardwareSerial::begin(unsigned long baud, uint32_t config)
{
	struct termios t;
	int ret = 0;

	if (_port_file == NULL)
		return;

	if (_fd > 0)
		close(_fd);

	_fd = open(_port_file, O_RDWR | O_NONBLOCK);
	if (_fd < 0)
		return;

	baud = baud ? baud : B115200;
	switch (baud) {
	case 0:
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
		ret = tcgetattr(_fd, &t);
		if (ret) {
			debug(DBG_OFF, "Fail to get attr for %s: %d\n", _port_file, ret);
			break;
		}

		t.c_cflag &= ~(CSIZE | CSTOPB | PARENB | PARODD);
		t.c_cflag |= config;
		cfsetispeed(&t, baud);

		ret = tcsetattr(_fd, TCSANOW, &t);
		if (ret) {
			debug(DBG_OFF, "Fail to set attr for %s: %d\n", _port_file, ret);
			break;
		}
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
	if (_fd)
		close(_fd);
	_fd = 0;
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
	/* TODO */
	return -1;
}

int HardwareSerial::read(void)
{
	unsigned char c;
	int ret;

	ret = read_byte(_fd, &c);
	if (ret <= 0)
		return -1;

	return c;
}

size_t HardwareSerial::write(uint8_t c)
{
	return write_byte(_fd, &c);
}

void HardwareSerial::flush(void)
{
	tcdrain(_fd);
}
