#include <Arduino.h>
#include <Modemif.h>

int new_smsidx;

void onSmsReady(void)
{
    /* ready for sms operation */
    /* delete all stored messages */
    Modem.smsDeleteAll();
}

void onNewSms(int index)
{
    /* new sms indication */
    new_smsidx = index;
}

void setup()
{
    Serial.begin(115200);
}

void loop()
{
	/* Rest of code */
	delay(500);
    if (new_smsidx > 0) {
        char sms_text[161], sms_num[21];
        int ret;

        ret = Modem.smsRead(new_smsidx, sms_num, sms_text);
        if (ret == 0) {
            Serial.print("New sms from: ");
            Serial.print(sms_num);
            Serial.print(": ");
            Serial.println(sms_text);
            /* delete message */
            Modem.smsDelete(new_smsidx);
            /* send reply */
            Modem.smsSend(sms_num, "I got your message");
        } else {
            Serial.print("Fail to read sms index ");
            Serial.print(new_smsidx);
            Serial.print(" error:");
            Serial.println(ret);
        }
        new_smsidx = 0;
    }
}