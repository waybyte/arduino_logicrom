#ifndef _VARIANT_SIM868_X_
#define _VARIANT_SIM868_X_

/*
 * Headers
 */
#include <hw/gpio.h>
#include <hw/adc.h>
#include <hw/pwm.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Default Definitions */
#define VARIANT_MCK 260000000
#define PWM_FREQUENCY 1000
#ifndef DEFAULT_STDIO_PORT
#define DEFAULT_STDIO_PORT "/dev/ttyUSB0"
#endif

#define HAS_SERIAL_UART2
#define HAS_USB_SERIAL
#define HAS_BT_SERIAL

#define SPI_MAX_SPEED 10000000UL

/* Analog Pin definition */
#define A0 GPIO_PIN_MAX

#define ADC_CHANNEL_MAX 1
#define ADC_CHANNEL_MAP {A0, ADC_CH0},

#define PWM_CHANNEL_MAX 2
#define PWM_CHANNEL_MAP {GPIO_5, PWM_CH0}, \
						{GPIO_26, PWM_CH1},

/* LED */
#define LED_BUILTIN GPIO_5

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 1
#define PIN_SPI_SS0 (GPIO_4)
#define PIN_SPI_MOSI (GPIO_15)
#define PIN_SPI_MISO (GPIO_14)
#define PIN_SPI_SCK (GPIO_6)
#define BOARD_SPI_SS0 (PIN_SPI_SS0)

static const uint8_t SS = BOARD_SPI_SS0;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK = PIN_SPI_SCK;

#ifdef __cplusplus
}
#endif

#endif
