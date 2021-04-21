/*
 * TwoWire.h - TWI/I2C library for Arduino Due
 * Copyright (c) 2011 Cristian Maglie <c.maglie@bug.st>.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Modified 2020 by Ajay Bhargav <contact@rickeyworld.info>
 */
#include "Arduino.h"
#include "Wire.h"

TwoWire::TwoWire(void) : rxBufferIndex(0), rxBufferLength(0), txAddress(0), txBufferLength(0), twiClock(TWI_CLOCK)
{
}

void TwoWire::begin(void)
{
	int ret;

	ret = i2c_hw_init(100);
#ifdef WIRE_DEBUG
	debug(DBG_OFF, "i2c_hw_init(%d) = %d\n", twiClock / 1000, ret);
#endif
	if (!ret)
		i2c_init_done = true;
}
	
void TwoWire::begin(uint8_t address)
{
	/* Do not support slave mode */
}

void TwoWire::begin(int address)
{
	begin((uint8_t)address);
}

void TwoWire::end(void)
{
	if (!i2c_init_done)
		return;

	i2c_hw_free();
	i2c_init_done = false;
}

void TwoWire::setClock(uint32_t frequency)
{
	twiClock = frequency;
	i2c_hw_setspeed(twiClock / 1000);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
	int read_bytes = 0, ret;

	if (!i2c_init_done)
		return 0;

	if (quantity > BUFFER_LENGTH)
		quantity = BUFFER_LENGTH;

	/* Read with repeated start not supported */
	if (!sendStop && rxBufferLength) {
		/* Please call EndTransmission() to send data */
		return 0;
	}

	if (txBufferLength) {
		/* write then read */
		ret = i2c_hw_writeread(address, txBuffer, txBufferLength, rxBuffer, quantity);
#ifdef WIRE_DEBUG
		debug(DBG_OFF, "i2c_hw_writeread(%x, %p, %d, %p, %d): %d\n", address, txBuffer, txBufferLength, rxBuffer, quantity, ret);
#endif
	} else {
		/* Just read */
		ret = i2c_hw_read(address, rxBuffer, quantity);
#ifdef WIRE_DEBUG
		debug(DBG_OFF, "i2c_hw_read(%x, %p, %d): %d\n", address, rxBuffer, quantity, ret);
#endif
	}

	if (ret < 0)
		rxBufferLength = 0;
	else
		rxBufferLength = ret;

	rxBufferIndex = 0;

	return ret;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) sendStop);
}

void TwoWire::beginTransmission(uint8_t address)
{
	if (!i2c_init_done)
		return;

	// save address of target and empty buffer
	txAddress = address;
	txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
	beginTransmission((uint8_t) address);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
	int ret;

	if (!i2c_init_done || (!sendStop && txBufferLength))
		return 4; /* Do not support write with repeated start */
	
	ret = i2c_hw_write(txAddress, txBuffer, txBufferLength);
#ifdef WIRE_DEBUG
	debug(DBG_OFF, "i2c_hw_write(%x, %p, %d) = %d\n", txAddress, txBuffer, txBufferLength, ret);
#endif
	if (ret < 0)
		return 4;

	txBufferLength = 0;

	return 0;
}

uint8_t TwoWire::endTransmission(void)
{
	return endTransmission(true);
}

size_t TwoWire::write(uint8_t data)
{
	if (!i2c_init_done || (txBufferLength >= BUFFER_LENGTH))
		return 0;

	txBuffer[txBufferLength++] = data;

	return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t len)
{
	if (!i2c_init_done)
		return 0;

	for (size_t i = 0; i < len; ++i) {
		if (txBufferLength >= BUFFER_LENGTH)
			return i;
		txBuffer[txBufferLength++] = data[i];
	}

	return len;
}

int TwoWire::available(void)
{
	return rxBufferLength - rxBufferIndex;
}

int TwoWire::read(void)
{
	if (rxBufferIndex < rxBufferLength)
		return rxBuffer[rxBufferIndex++];
	return -1;
}

int TwoWire::peek(void)
{
	if (rxBufferIndex < rxBufferLength)
		return rxBuffer[rxBufferIndex];
	return -1;
}

void TwoWire::flush(void)
{
	/* use endTransmission() to send data */
}

void TwoWire::onReceive(void (*function)(int))
{
	onReceiveCallback = function;
}

void TwoWire::onRequest(void (*function)(void))
{
	onRequestCallback = function;
}

void TwoWire::onService(void)
{
	/* Nothing to do */
}

TwoWire Wire = TwoWire();
