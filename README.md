# Logicrom Arduino
Arduino framework for Logicrom OpenCPU Development Platform

# Installation

## Add Board to Arduino Board Manager
* Open Arduino IDE
* Go To **File** &#707; **Preferences** &#707;
* Add URL to _Additional Board Manager URLs_: https://waybyte.github.io/package_logicrom_index.json
* Click OK

## Install Board
* Go To Arduino IDE Menu, **Tools** &#707; **Board** &#707; **Board Manager**
* In search box enter: _logic_
* Click Install for _Logicrom Arduino Boards_

# Logicrom Board Configuration

## Build Type
User can select either Release or Debug from build type.
Debug build will enable extra debug messages from library package.

## STDIO Port Selection
User can select standard I/O port for `printf` function from following options
* Uart0 `/dev/ttyS0`
* Uart1 `/dev/ttyS1`
* Uart2 `/dev/ttyS2`
* USB Uart `/dev/ttyUSB0`

## Complete Installation Guide
 - [PlatformIO IDE](https://docs.logicrom.com/en/latest/book/quick_start/setup_platformio.html)
 - [Arduino IDE](https://docs.logicrom.com/en/latest/book/quick_start/setup_arduino.html)

## Resources

* [Documentation](https://docs.logicrom.com) - Logicrom SDK documentation for latest version
* [SDK Examples](https://docs.logicrom.com/en/latest/book/example.html)

# Supported Modules

| Name              | Vendor   | SoC/Chipset | Supported Peripherals                              | APP RAM / ROM   |
|-------------------|----------|-------------|----------------------------------------------------|-----------------|
| EC200U-XX-YY [^1] | Quectel  | RDA8910     |  UART, USB, SPI, I2C, ADC, LCD, BT [^2], GNSS [^3] | 500 KB / 1 MB   |
| EC600U-XX-YY [^1] | Quectel  | RDA8910     |                                                    | 500 KB / 1 MB   |
| EC600U-XX-YY [^1] | Quectel  | RDA8910     |                                                    | 500 KB / 1 MB   |
| EG915U-XX-YY [^1] | Quectel  | RDA8910     |                                                    | 500 KB / 1 MB   |
| N58-CA            | Neoway   | RDA8910     |                                                    | 500 KB / 1 MB   |
| N716-CA           | Neoway   | RDA8910     |                                                    | 500 KB / 1 MB   |
| EC100N-XX-XX      | Quectel  | ASR1603     |  UART, USB, SPI, I2C, ADC                          | 512 KB / 1 MB   |
| EC200N-CN-AA      | Quectel  | ASR1603     |                                                    | 512 KB / 512 KB |
| EC600N-CN-AA      | Quectel  | ASR1603     |                                                    | 512 KB / 1 MB   |
| EG912Y-EU-YY      | Quectel  | ASR1603     |                                                    | 512 KB / 1 MB   |
| EG915N-EU-YY      | Quectel  | ASR1603     |                                                    | 512 KB / 512 KB |
| EC100Y-CN-YY      | Quectel  | ASR1601     |  UART, USB, SPI, I2C, ADC                          | 512 KB / 512 KB |
| EC100S-CN-YY      | Quectel  | ASR1601     |                                                    | 512 KB / 1 MB   |
| EC600S-CN-YY      | Quectel  | ASR1601     |                                                    | 512 KB / 1 MB   |
| AC7670C           | SIMCOM   | ASR1601     |                                                    | 512 KB / 1 MB   |
| MC20, MC60, MC20U | Quectel  | MT2503      |  UART, USB, SPI, I2C, ADC, GNSS                    | 94 KB / 256 KB  |
| SIM868[^4]        | SIMCOM   | MT2503      |                                                    | 94 KB / 256 KB  |
| M66, M26, M56     | Quectel  | MT6261      |  UART, USB, SPI, I2C, ADC                          | 94 KB / 256 KB  |
| SIM800[^4]        | SIMCOM   | MT6261      |                                                    | 94 KB / 256 KB  |
| MC65              | Quectel  | RDA8955     |  UART, USB, SPI, I2C, ADC, GNSS [^3], LCD          | 1 MB / 576 KB   |
| M590              | Neoway   | RDA8955     |                                                    | 1 MB / 576 KB   |
| A9, A9G           | AiThinker| RDA8955     |                                                    | 1 MB / 576 KB   |


[^1]: XX can be CN/AU/EU, YY can be AA/AB/AC

[^2]: Currely only GATT Server supported and used for console purpose only

[^3]: Supported on module with GNSS

[^4]: IMEI need to be configured when core is flashed for first time, use AT+EGMR=1,7,"imei"

> DFOTA is also supported for LTE modules
