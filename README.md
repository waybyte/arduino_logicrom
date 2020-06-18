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
