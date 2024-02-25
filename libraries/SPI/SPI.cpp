/*
 * SPI Master library for Logicrom Platform
 * Copyright (c) 2015 Arduino LLC
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
 * Modified by Ajay Bhargav <ajay.bhargav@waybyte.in>
 * 
 */

#include <Arduino.h>
#include <assert.h>
#include "SPI.h"

#ifndef SPI_DEFAULT_PORT
#define SPI_DEFAULT_PORT SPI_PORT_0
#endif

SPIClass::SPIClass(int port) : settings(SPISettings(10000000, MSBFIRST, SPI_MODE0))
{
	this->port = port;
	initialized = false;
}

void SPIClass::begin()
{
	init();
}

void SPIClass::init()
{
	if (initialized)
		return;

	spi_hw_init(port, FALSE, settings.clockFreq, settings.dataMode, SPI_CSPOL_LOW);

	initialized = true;
}

void SPIClass::config(SPISettings settings)
{
	if (this->settings != settings)
	{
		this->settings = settings;

		spi_hw_setclock(port, settings.clockFreq);
		spi_hw_setmode(port, settings.dataMode);
		spi_hw_setbitorder(port, settings.bitOrder == MSBFIRST ? SPI_MSB_FIRST : SPI_LSB_FIRST);
	}
}

void SPIClass::end()
{
	if (initialized) {
		spi_hw_free(port);
		initialized = false;
	}
}

void SPIClass::beginTransaction(SPISettings settings)
{
	config(settings);
}

void SPIClass::endTransaction(void)
{
}

void SPIClass::setBitOrder(BitOrder order)
{
	settings.bitOrder = order;
	spi_hw_setbitorder(port, order == MSBFIRST ? SPI_MSB_FIRST : SPI_LSB_FIRST);
}

void SPIClass::setDataMode(uint8_t mode)
{
	settings.dataMode = mode;
	spi_hw_setmode(port, mode);
}

void SPIClass::setClockDivider(uint8_t div)
{
	spi_hw_setclock(port, SPI_MAX_SPEED / div);
}

byte SPIClass::transfer(uint8_t data)
{
	uint32_t out_byte;

	spi_hw_transfer(port, &data, (unsigned char *)&out_byte, 1);

	return out_byte;
}

uint16_t SPIClass::transfer16(uint16_t data)
{
	union {
		uint16_t val;
		struct
		{
			uint8_t lsb;
			uint8_t msb;
		};
	} t;

	t.val = data;

	if (settings.bitOrder == LSBFIRST)
	{
		t.lsb = transfer(t.lsb);
		t.msb = transfer(t.msb);
	}
	else
	{
		t.msb = transfer(t.msb);
		t.lsb = transfer(t.lsb);
	}

	return t.val;
}

void SPIClass::transfer(void *buf, size_t count)
{
	uint8_t *buffer = reinterpret_cast<uint8_t *>(buf);
	for (size_t i = 0; i < count; i++)
	{
		*buffer = transfer(*buffer);
		buffer++;
	}
}

#ifdef SOC_RDA8910
void SPIClass::CSControl(bool level)
{
	spi_hw_cscontrol(port, level);
}
#endif

SPIClass SPI = SPIClass(SPI_DEFAULT_PORT);
