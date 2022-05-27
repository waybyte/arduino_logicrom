# Logicrom Arduino
Arduino framework for Logicrom Wireless IoT Platform

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

## Resources

* [Documentation](https://docs.logicrom.com) - Logicrom SDK documentation for latest version

# Supported Modules
## 4G LTE Cat.1 Modules

| Module Name  | Networking | BLE[^1] | GPS | GPIO | ADC | I2C | SPI | USB | LCD | Camera |
|--------------|------------|---------|-----|------|-----|-----|-----|-----|-----|--------|
| Neoway N58  | &check; | &check; | &check; | &check; | &check; | &check; | &check; | &check; | &check; | &cross; |
| Neoway N716 | &check; | &check; | &#8212; | &check; | &check; | &check; | &check; | &check; | &check; | &cross; |
| Quectel EC200U | &check; | &check; | &check; | &check; | &check; | &check; | &check; | &check; | &check; | &cross; |
| Quectel EC600U | &check; | &check; | &#8212; | &check; | &check; | &check; | &check; | &check; | &check; | &cross; |
| Quectel EG915U  | &check; | &check; | &#8212; | &check; | &check; | &check; | &check; | &check; | &check; | &cross; |
| Fibocom L610  | &check; | &check; | &#8212; | &check; | &check; | &check; | &check; | &check; | &check; | &cross; |

> DFOTA is also supported for LTE modules

## NB-IoT Modules[^2]

| Module Name  | Networking | GPS | GPIO | ADC | I2C | SPI | USB |
|--------------|------------|-----|------|-----|-----|-----|-----|
| Quectel BC66 | &check; | &#8212; | &check; | &check; | &check; | &check; | &check; |
| Quectel BC20 | &check; | &check; | &check; | &check; | &check; | &check; | &check; |

## GSM Modules

| Module Name  | Networking | BT | GPS | GPIO | ADC | I2C | SPI | USB | LCD[^3] |
|--------------|------------|----|-----|------|-----|-----|-----|-----|---------|
| Quectel M66  | &check; | &check; | &#8212; | &check; | &check; | &check; | &check; | &#8212; | &cross; |
| Quectel M66DS| &check; | &check; | &#8212; | &check; | &check; | &check; | &check; | &#8212; | &cross; |
| Quectel MC60 | &check; | &check; | &check; | &check; | &check; | &check; | &check; | &#8212; | &cross; |
| Quectel MC20 | &check; | &check; | &check; | &check; | &check; | &check; | &check; | &#8212; | &cross; |
| Quectel M56| &check; | &check; | &#8212; | &check; | &check; | &check; | &check; | &check; | &cross; |
| Quectel MC20U| &check; | &check; | &check; | &check; | &check; | &check; | &check; | &check; | &cross; |
| SIMCOM SIM868[^4]| &check; | &check; | &check; | &check; | &check; | &check; | &check; | &check; | &cross; |


[^1]: Currely only GATT Server supported and used for console purpose only.

[^2]: Updates to come

[^3]: LCD is work in progress.

[^4]: Please backup calibration during first flash via Maui Meta tool (google is your friend).
