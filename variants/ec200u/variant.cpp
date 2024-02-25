#ifdef PLATFORM_LOGICROM_SPARK
#include <hw/gpio.h>
#include <network.h>

static int logicrom_spark_iomap[] {
    GPIO_PIN_MAX,
    GPIO_PIN_MAX,
    GPIO_24,
    GPIO_10,
    GPIO_25,
    GPIO_26,
    GPIO_27,
    GPIO_28,
    GPIO_18_ALT,
    GPIO_19_ALT,
    GPIO_23,
    GPIO_2,
    GPIO_3,
    GPIO_0,
    GPIO_17,
    GPIO_16,
    GPIO_11,
    GPIO_12,
    GPIO_20_ALT,
    GPIO_21_ALT,
    GPIO_5,
    GPIO_13,
    GPIO_30,
    GPIO_29,
    GPIO_31,
};

extern "C" int io_pin2gpio(int pin)
{
    if (pin >= (sizeof(logicrom_spark_iomap) / sizeof(*logicrom_spark_iomap)))
        return GPIO_PIN_MAX;
    return logicrom_spark_iomap[pin];
}

void variant_init(void)
{
    /* setup network LED for Spark board */
    network_setup_statusled(GPIO_22);
}
#endif
