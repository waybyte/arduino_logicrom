/*
 * SPI Master library for SIWI GSM Modules
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

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <Arduino.h>

/*
 * SPI_HAS_TRANSACTION means SPI has
 *  - beginTransaction()
 *  - endTransaction()
 *  - SPISetting(clock, bitOrder, dataMode)
 */
#define SPI_HAS_TRANSACTION 1

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define SPI_CLOCK_DIV2 2
#define SPI_CLOCK_DIV4 4
#define SPI_CLOCK_DIV8 8
#define SPI_CLOCK_DIV16 16
#define SPI_CLOCK_DIV32 32
#define SPI_CLOCK_DIV64 64
#define SPI_CLOCK_DIV128 128

class SPISettings
{
public:
	SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode)
	{
		this->clockFreq = clock;
		this->bitOrder = bitOrder;
		this->dataMode = dataMode;
	}

	/* Default speed set to 10MHz, SPI mode set to MODE 0 and Bit order set to MSB first. */
	SPISettings()
	{
		this->clockFreq = 10000000;
		this->bitOrder = MSBFIRST;
		this->dataMode = SPI_MODE0;
	}

	bool operator==(const SPISettings &rhs) const
	{
		if ((this->clockFreq == rhs.clockFreq) &&
			(this->bitOrder == rhs.bitOrder) &&
			(this->dataMode == rhs.dataMode))
		{
			return true;
		}
		return false;
	}

	bool operator!=(const SPISettings &rhs) const
	{
		return !(*this == rhs);
	}

	void operator=(const SPISettings &rhs)
	{
		this->clockFreq = rhs.clockFreq;
		this->bitOrder = rhs.bitOrder;
		this->dataMode = rhs.dataMode;
	}

	uint32_t getClockFreq() const { return clockFreq; }
	uint8_t getDataMode() const { return dataMode; }
	BitOrder getBitOrder() const { return bitOrder; }

private:
	uint32_t clockFreq;
	uint8_t dataMode;
	BitOrder bitOrder;

	friend class SPIClass;
};

const SPISettings DEFAULT_SPI_SETTINGS = SPISettings();

class SPIClass
{
public:
	SPIClass();

	byte transfer(uint8_t data);
	uint16_t transfer16(uint16_t data);
	void transfer(void *buf, size_t count);

	/* Transaction Functions */
	void usingInterrupt(int interruptNumber);
	void notUsingInterrupt(int interruptNumber);
	void beginTransaction(SPISettings settings);
	void endTransaction(void);

	void begin();
	void end();

	void setBitOrder(BitOrder order);
	void setDataMode(uint8_t uc_mode);
	void setClockDivider(uint8_t uc_div);

private:
	void init();
	void config(SPISettings settings);

	SPISettings settings;
	bool initialized;
	int chip_select;
};

extern SPIClass SPI;

#endif
