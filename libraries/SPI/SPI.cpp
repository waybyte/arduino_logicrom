/*
 * SPI Master library for LOGICROM Platform
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
 * Modified by Ajay Bhargav <contact@rickeyworld.info>
 * 
 */

#include <Arduino.h>
#include <assert.h>
#include "SPI.h"

SPIClass::SPIClass(void) : settings(SPISettings(10000000, MSBFIRST, SPI_MODE0))
{
	initialized = false;
}

void SPIClass::begin()
{
	init();
	config(settings);
}

void SPIClass::init()
{
	if (initialized)
		return;

	spi_hw_init(FALSE);

	initialized = true;
}

void SPIClass::config(SPISettings settings)
{
	if (this->settings != settings)
	{
		this->settings = settings;

		spi_hw_setclock(settings.clockFreq);
		spi_hw_setmode(settings.dataMode);
		spi_hw_setbyteorder(settings.bitOrder == MSBFIRST ? SPI_BYTE_MSB_FIRST : SPI_BYTE_LSB_FIRST);
	}
}

void SPIClass::end()
{
	if (initialized) {
		spi_hw_free();
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
	spi_hw_setbyteorder(order == MSBFIRST ? SPI_BYTE_MSB_FIRST : SPI_BYTE_LSB_FIRST);
}

void SPIClass::setDataMode(uint8_t mode)
{
	settings.dataMode = mode;
	spi_hw_setmode(mode);
}

void SPIClass::setClockDivider(uint8_t div)
{
	spi_hw_setclock(SPI_MAX_SPEED / div);
}

byte SPIClass::transfer(uint8_t data)
{
	uint32_t out_byte;

	spi_hw_transfer(&data, (unsigned char *)&out_byte, 1);

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

SPIClass SPI = SPIClass();
