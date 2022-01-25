# Logicrom Arduino
Arduino framework for Logicrom

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
Make sure _S20 GSM Board_ is selected from Board Manager after Installation.

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
| Neoway N58  | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: | :x: |
| Neoway N716 | :heavy_check_mark: | :heavy_check_mark: | :heavy_minus_sign: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: | :x: |
| Quectel EC200U | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: | :x: |
| Quectel EC600U | :heavy_check_mark: | :heavy_check_mark: | :heavy_minus_sign: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: | :x: |
| Fibocom L610  | :heavy_check_mark: | :heavy_check_mark: | :heavy_minus_sign: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: | :x: |

DFOTA is also supported of on above mentioned LTE modules

## NB-IoT Modules[^2]

| Module Name  | Networking | GPS | GPIO | ADC | I2C | SPI | USB |
|--------------|------------|-----|------|-----|-----|-----|-----|
| Quectel BC66 | :heavy_check_mark: | :heavy_minus_sign: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| Quectel BC20 | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |

## GSM Modules

| Module Name  | Networking | BT | GPS | GPIO | ADC | I2C | SPI | USB | LCD[^3] |
|--------------|------------|----|-----|------|-----|-----|-----|-----|---------|
| Quectel M66  | :heavy_check_mark: | :heavy_check_mark: | :heavy_minus_sign: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_minus_sign: | :x: |
| Quectel M66DS| :heavy_check_mark: | :heavy_check_mark: | :heavy_minus_sign: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_minus_sign: | :x: |
| Quectel MC60 | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_minus_sign: | :x: |
| Quectel MC20 | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_minus_sign: | :x: |
| Quectel M56| :heavy_check_mark: | :heavy_check_mark: | :heavy_minus_sign: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: |
| Quectel MC20U| :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: |
| SIMCOM SIM868[^4]| :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: |

[^1]: Currely only GATT Server supported and used for console purpose only.
[^2]: Updates to come
[^3]: LCD is work in progress.
[^4]: Please backup calibration during first flash via Maui Meta tool (google is your friend).
