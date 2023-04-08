#ifndef _VARIANT_ASR160X_X_
#define _VARIANT_ASR160X_X_

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
#define VARIANT_MCK 500000000
#define PWM_FREQUENCY 1000
#ifndef DEFAULT_STDIO_PORT
#define DEFAULT_STDIO_PORT "/dev/ttyUSB0"
#endif

#define HAS_SERIAL_UART2
#define HAS_USB_SERIAL

#define SPI_MAX_SPEED 40000000UL

/* Analog Pin definition */
#define A0 GPIO_PIN_MAX
#define A1 GPIO_PIN_MAX + 1

#define ADC_CHANNEL_MAX 2
#define ADC_CHANNEL_MAP {A0, ADC_CH0}, \
						{A1, ADC_CH1},

#define PWM_CHANNEL_MAX 10
#define PWM_CHANNEL_MAP {GPIO_8, PWM_CH0_IO8},   \
						{GPIO_31, PWM_CH0_IO31}, \
						{GPIO_9, PWM_CH1_IO9},   \
						{GPIO_32, PWM_CH1_IO32}, \
						{GPIO_6, PWM_CH2_IO6},   \
						{GPIO_8, PWM_CH2_IO8},   \
						{GPIO_10, PWM_CH2_IO10}, \
						{GPIO_7, PWM_CH3_IO7},   \
						{GPIO_9, PWM_CH3_IO9},   \
						{GPIO_11, PWM_CH3_IO11},

/* LED - check board for pin information */
#define LED_BUILTIN GPIO_0

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 2
#define PIN_SPI_SCK (GPIO_16)
#define PIN_SPI_SS0 (GPIO_17)
#define PIN_SPI_MISO (GPIO_18)
#define PIN_SPI_MOSI (GPIO_19)
#define BOARD_SPI_SS0 (PIN_SPI_SS0)

static const uint8_t SS = BOARD_SPI_SS0;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK = PIN_SPI_SCK;

#ifdef __cplusplus
}
#endif

#endif
