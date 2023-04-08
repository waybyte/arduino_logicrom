#ifndef _VARIANT_MC60_X_
#define _VARIANT_MC60_X_

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
#define DEFAULT_STDIO_PORT "/dev/ttyS0"
#endif

#define HAS_SERIAL_UART2

#define SPI_MAX_SPEED 10000000UL

/* Analog Pin definition */
#define A0 GPIO_PIN_MAX
#define A1 GPIO_0
#define A2 GPIO_26
#define A3 GPIO_27

#define ADC_CHANNEL_MAX 4
#define ADC_CHANNEL_MAP {A0, ADC_CH0}, \
						{A1, ADC_CH1}, \
						{A2, ADC_CH2}, \
						{A3, ADC_CH3},

#define PWM_CHANNEL_MAX 2
#define PWM_CHANNEL_MAP {GPIO_0, PWM_CH0}, \
						{GPIO_31, PWM_CH1},

/* LED */
#define LED_BUILTIN GPIO_0

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 1
#define PIN_SPI_SS0 (GPIO_6)
#define PIN_SPI_MISO (GPIO_7)
#define PIN_SPI_SCK (GPIO_8)
#define PIN_SPI_MOSI (GPIO_9)
#define BOARD_SPI_SS0 (PIN_SPI_SS0)

static const uint8_t SS = BOARD_SPI_SS0;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK = PIN_SPI_SCK;

#ifdef __cplusplus
}
#endif

#endif
