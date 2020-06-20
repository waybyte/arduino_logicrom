# siwiduino
Arduino framework for SIWI GSM modules

# Installation

## Add Board to Arduino Board Manager
* Open Arduino IDE
* Go To **File** &#707; **Preferences** &#707;
* Add URL to _Additional Board Manager URLs_: https://siwiembedded.github.io/package_siwigsm_index.json
* Click OK

## Install Board
* Go To Arduino IDE Menu, **Tools** &#707; **Board** &#707; **Board Manager**
* In search box enter: _siwigsm_
* Click Install for _SIWIGSM Arduino Boards_

# SIWIGSM Board Configuration
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

# Supported Platforms
## SIWI S20U GSM Module
* S20U Module
* S20 GSM development Board
<img src="https://siwiembedded.github.io/asset/s20gsm_board.png" alt="s20 gsm board" width="50%">

## Quectel M66 2G GSM
<img src="https://www.quectel.com/UploadImage/Product/20200304152709774.png" alt="Quectel M66" width="50%">

## Quectel MC60 2G GSM
<img src="https://www.quectel.com/UploadImage/Product/20200302164806327.png" alt="Quectel MC60" width="50%">

## Quectel MC20 2G GSM
<img src="https://www.quectel.com/UploadImage/Product/20200302175738828.png" alt="Quectel MC20" width="50%">

## Quectel M56 GSM
<img src="https://www.quectel.com/UploadImage/Product/20200304115535722.png" alt="Quectel M56" width="50%">

