#ifndef __MODEM_INTERFACE_H__
#define __MODEM_INTERFACE_H__

#include <Arduino.h>
#include <modem.h>
#include <ril.h>

class ModemInterface
{
private:
    /* data */
public:
    ModemInterface();
    ~ModemInterface();

    void enableAirplaneMode(void);
    void disableAirplaneMode(void);
	/* system information */
	const char *getIMEI(void);
	const char *getIMSI(void);
	const char *getICCID(void);
    /* sms */
    int smsRead(int idx, char *text);
    int smsRead(int idx, char *num, char *text);
    int smsRead(int idx, char *num, char *text, struct tm *timestamp);
    int smsSend(const char *num, const char *text);
    int smsDelete(int index);
    int smsDeleteAll(void);
    /* call */
    int callDial(const char *num);
    int callAnswer(void);
    int callHangup(void);
    /* LBS information */
    int getLBSInformation(struct md_cellinfo_t *info);
};

extern ModemInterface Modem;

#endif
