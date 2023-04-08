#include <Arduino.h>
#include <Modemif.h>

void onIncomingCall(const char *number)
{
    /* Incoming call indication */
    Serial.print("Incoming call from: ");
    Serial.println(number);
    /* Anser call */
    Modem.callAnswer();
}

void onCallStateChange(int state)
{
    switch (state)
    {
    case CALL_STATE_BUSY:
        break;
    case CALL_STATE_NO_ANSWER:
        break;
    case CALL_STATE_NO_CARRIER:
        break;
    case CALL_STATE_NO_DIALTONE:
        break;
    default:
        break;
    }
}

void loop()
{
	/* Rest of code */
	delay(1000);
}
