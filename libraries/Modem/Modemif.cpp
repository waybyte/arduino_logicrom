#include <string.h>
#include "Modemif.h"

ModemInterface::ModemInterface()
{
}

ModemInterface::~ModemInterface()
{
}

void ModemInterface::enableAirplaneMode(void)
{
    md_cfun_set(4, 0);
}

void ModemInterface::disableAirplaneMode(void)
{
    md_cfun_set(1, 0);
}

const char *ModemInterface::getIMEI(void)
{
	return md_get_imei(NULL, 0);
}

const char *ModemInterface::getIMSI(void)
{
	return md_get_imsi(NULL, 0);
}

const char *ModemInterface::getICCID(void)
{
	return md_get_ccid(NULL, 0);
}

int ModemInterface::smsRead(int idx, char *text)
{
    struct md_smsinfo_t info;

    int ret = md_sms_read(idx, &info);
    if (ret)
        return ret;
    
    strcpy(text, info.text);

    return 0;
}

int ModemInterface::smsRead(int idx, char *num, char *text)
{
    struct md_smsinfo_t info;

    int ret = md_sms_read(idx, &info);
    if (ret)
        return ret;
    
    strcpy(num, info.oa_num);
    strcpy(text, info.text);

    return 0;
}

int ModemInterface::smsRead(int idx, char *num, char *text, struct tm *timestamp)
{
    struct md_smsinfo_t info;

    int ret = md_sms_read(idx, &info);
    if (ret)
        return ret;
    
    strcpy(num, info.oa_num);
    strcpy(text, info.text);
    memcpy(timestamp, &info.ts, sizeof(*timestamp));

    return 0;
}

int ModemInterface::smsSend(const char *num, const char *text)
{
    unsigned int ref;

    return md_sms_send(num, text, &ref);
}

int ModemInterface::smsDelete(int index)
{
    return md_sms_delete(index, SMS_DEL_INDEXED);
}

int ModemInterface::smsDeleteAll(void)
{
    return md_sms_delete(1, SMS_DEL_ALL);
}

int ModemInterface::callDial(const char *num)
{
    return md_call_dial(num);
}

int ModemInterface::callAnswer(void)
{
    return md_call_answer();
}

int ModemInterface::callHangup(void)
{
    return md_call_hangup();
}

int ModemInterface::getLBSInformation(struct md_cellinfo_t *info)
{
    return md_get_cellinfo(info);
}

ModemInterface Modem;
