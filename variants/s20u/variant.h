#ifndef _VARIANT_S20U_X_
#define _VARIANT_S20U_X_

/*
 * Headers
 */
#include <lib.h>
#include <utils.h>
#include <os_api.h>
#include <hw/gpio.h>
#include <hw/adc.h>
#include <hw/pwm.h>
#include <hw/i2c.h>
#include <hw/spi.h>

#include <ril.h>

/* Default Definitions */
#define VARIANT_MCK 260000000
#define PWM_FREQUENCY 1000
#ifndef DEFAULT_STDIO_PORT
#define DEFAULT_STDIO_PORT "/dev/ttyUSB0"
#endif

/* Analog Pin definition */
#define A0 GPIO_PIN_MAX
#define A1 GPIO_0
#define A2 GPIO_20
#define A3 GPIO_30
#define A4 GPIO_31
#ifdef PLATFORM_M56
#define A5 (GPIO_PIN_MAX + 1)
#endif

/* LED */
#define LED_BUILTIN GPIO_0

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 1
#define PIN_SPI_SS0 (GPIO_6)
#define PIN_SPI_MOSI (GPIO_9)
#define PIN_SPI_MISO (GPIO_7)
#define PIN_SPI_SCK (GPIO_8)
#define BOARD_SPI_SS0 (PIN_SPI_SS0)

static const uint8_t SS = BOARD_SPI_SS0;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK = PIN_SPI_SCK;

/* For Variant.cpp */
#define CHECK_MODE(pin, mode) ((g_ioModes[pin] & 0xff) == IO_MODE_##mode)

enum _io_mode {
	IO_MODE_GPIO = 1,
	IO_MODE_ADC = 2,
	IO_MODE_PWM = 3,
};

#ifdef __cplusplus
extern "C"
{
#endif

extern int g_ioHandles[GPIO_PIN_MAX];
extern int g_ioModes[GPIO_PIN_MAX];

int pin2adc_channel(uint32_t pin);
int pin2pwm_channel(uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif
