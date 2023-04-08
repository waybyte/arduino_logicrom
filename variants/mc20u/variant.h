#ifndef _VARIANT_MC20U_X_
#define _VARIANT_MC20U_X_

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

#define SPI_MAX_SPEED 10000000UL

/* Analog Pin definition */
#define A0 GPIO_PIN_MAX
#define A1 GPIO_0
#define A2 GPIO_20
#define A3 GPIO_30
#define A4 GPIO_31
#ifdef PLATFORM_M56
#define A5 (GPIO_PIN_MAX + 1)
#endif

#ifdef PLATFORM_M56
#define ADC_CHANNEL_MAX 6
#define ADC_CHANNEL_MAP {A0, ADC_CH0}, \
						{A1, ADC_CH1}, \
						{A2, ADC_CH2}, \
						{A3, ADC_CH3}, \
						{A4, ADC_CH4}, \
						{A5, ADC_CH5},
#else
#define ADC_CHANNEL_MAX 5
#define ADC_CHANNEL_MAP {A0, ADC_CH0}, \
						{A1, ADC_CH1}, \
						{A2, ADC_CH2}, \
						{A3, ADC_CH3}, \
						{A4, ADC_CH4},
#endif

#define PWM_CHANNEL_MAX 2
#define PWM_CHANNEL_MAP {GPIO_0, PWM_CH0}, \
						{GPIO_35, PWM_CH1},

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
