#include "Arduino.h"
#include "wiring_private.h"

static int g_ioHandles[GPIO_PIN_MAX];
static int g_ioModes[GPIO_PIN_MAX];

static const uint32_t adc_channel_map[ADC_CHANNEL_MAX][2] = {
    ADC_CHANNEL_MAP
};

static const uint32_t pwm_channel_map[PWM_CHANNEL_MAX][2] = {
    PWM_CHANNEL_MAP
};

int io_pinvalid(pin_size_t pin)
{
    return pin < GPIO_PIN_MAX;
}

int io_gethandle(pin_size_t pin)
{
    if (pin >= GPIO_PIN_MAX)
        return 0;
    
    return g_ioHandles[pin];
}

void io_sethandle(pin_size_t pin, int handle)
{
    if (pin >= GPIO_PIN_MAX)
        return;
    
    g_ioHandles[pin] = handle;
}

int io_getmode(pin_size_t pin)
{
    if (pin >= GPIO_PIN_MAX)
        return 0;
    
    return g_ioModes[pin];
}

void io_setmode(pin_size_t pin, uint32_t mode)
{
    if (pin < GPIO_PIN_MAX) {
        g_ioModes[pin] = mode;
    }
}

uint8_t io_checkmode(pin_size_t pin, uint32_t mode)
{
    if (pin < GPIO_PIN_MAX) {
        return ((g_ioModes[pin] & 0xff) == mode);
    }
    return FALSE;
}

int pin2adc_channel(uint32_t pin)
{
	for (int i = 0; i < ADC_CHANNEL_MAX; i++) {
		if (adc_channel_map[i][0] == pin)
			return adc_channel_map[i][1];
	}

	return -1;
}

int pin2pwm_channel(uint32_t pin)
{
	for (int i = 0; i < PWM_CHANNEL_MAX; i++) {
		if (pwm_channel_map[i][0] == pin)
			return pwm_channel_map[i][1];
	}

	return -1;
}
