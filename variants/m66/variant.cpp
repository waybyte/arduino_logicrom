/*
  Copyright (c) 2020 SiWi Embedded Solutions.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License..

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
   See the GNU Lesser General Public License for more details.
*/

#include "Arduino.h"

#ifdef __cplusplus
extern "C"
{
#endif

int g_ioHandles[GPIO_PIN_MAX];
int g_ioModes[GPIO_PIN_MAX];

static const uint32_t adc_channel_map[4][2] = {
	{GPIO_1, ADC_CH1},
	{GPIO_7, ADC_CH2},
	{GPIO_8, ADC_CH3},
	{GPIO_PIN_MAX, ADC_CH0},
};

int pin2adc_channel(uint32_t pin)
{
	for (int i = 0; i < 4; i++) {
		if (adc_channel_map[i][0] == pin)
			return adc_channel_map[i][1];
	}

	return -1;
}

int pin2pwm_channel(uint32_t pin)
{
	if (pin == GPIO_1)
		return PWM_CH0;

	return -1;
}

#ifdef __cplusplus
}
#endif

static void urc_callback(unsigned int param1, unsigned int param2)
{
	switch (param1)
	{
	case URC_SYS_INIT_STATE_IND:
		if (param2 == SYS_STATE_SMSOK)
		{
			/* Ready for SMS */
		}
		break;
	case URC_SIM_CARD_STATE_IND:
		switch (param2)
		{
		case SIM_STAT_NOT_INSERTED:
			debug(DBG_OFF, "SYSTEM: SIM card not inserted!\n");
			break;
		case SIM_STAT_READY:
			debug(DBG_INFO, "SYSTEM: SIM card Ready!\n");
			break;
		case SIM_STAT_PIN_REQ:
			debug(DBG_OFF, "SYSTEM: SIM PIN required!\n");
			break;
		case SIM_STAT_PUK_REQ:
			debug(DBG_OFF, "SYSTEM: SIM PUK required!\n");
			break;
		case SIM_STAT_NOT_READY:
			debug(DBG_OFF, "SYSTEM: SIM card not recognized!\n");
			break;
		default:
			debug(DBG_OFF, "SYSTEM: SIM ERROR: %d\n", param2);
		}
		break;
	case URC_GSM_NW_STATE_IND:
		debug(DBG_OFF, "SYSTEM: GSM NW State: %d\n", param2);
		break;
	case URC_GPRS_NW_STATE_IND:
		break;
	case URC_CFUN_STATE_IND:
		break;
	case URC_COMING_CALL_IND:
		debug(DBG_OFF, "Incoming voice call from: %s\n", ((ST_ComingCall *)param2)->phoneNumber);
		/* Take action here, Answer/Hang-up */
		break;
	case URC_CALL_STATE_IND:
		switch (param2)
		{
		case CALL_STATE_BUSY:
			debug(DBG_OFF, "The number you dialed is busy now\n");
			break;
		case CALL_STATE_NO_ANSWER:
			debug(DBG_OFF, "The number you dialed has no answer\n");
			break;
		case CALL_STATE_NO_CARRIER:
			debug(DBG_OFF, "The number you dialed cannot reach\n");
			break;
		case CALL_STATE_NO_DIALTONE:
			debug(DBG_OFF, "No Dial tone\n");
			break;
		default:
			break;
		}
		break;
	case URC_NEW_SMS_IND:
		debug(DBG_OFF, "SMS: New SMS (%d)\n", param2);
		/* Handle New SMS */
		break;
	case URC_MODULE_VOLTAGE_IND:
		debug(DBG_INFO, "VBatt Voltage: %d\n", param2);
		break;
	case URC_ALARM_RING_IND:
		break;
	case URC_FILE_DOWNLOAD_STATUS:
		break;
	case URC_FOTA_STARTED:
		break;
	case URC_FOTA_FINISHED:
		break;
	case URC_FOTA_FAILED:
		break;
	case URC_STKPCI_RSP_IND:
		break;
	default:
		break;
	}
}

void variant_init(void)
{
	/* Initialize Siwi system task */
	siwilib_init(DEFAULT_STDIO_PORT, urc_callback);
}
