#include <Arduino.h>
#include <Net.h>
#include <Modemif.h>

void onSimStateChange(int state)
{
    switch (state)
    {
    case SIM_STAT_NOT_INSERTED:
        break;
    case SIM_STAT_READY:
        break;
    case SIM_STAT_PIN_REQ:
        break;
    case SIM_STAT_PUK_REQ:
        break;
    case SIM_STAT_NOT_READY:
       break;
    default:
        break;
    }
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

void onGSMStateChange(int state)
{
    switch (state)
    {
    case NW_STAT_NOT_REGISTERED:
        break;
    case NW_STAT_REGISTERED_HOME:
        break;
    case NW_STAT_SEARCHING:
        break;
    case NW_STAT_REG_DENIED:
        break;
    case NW_STAT_UNKNOWN:
        break;
    case NW_STAT_REGISTERED_ROAMING:
        break;
    case NW_STAT_SMSONLY_HOME:
        break;
    case NW_STAT_SMSONLY_ROAMING:
        break;
    default:
        break;
    }
}

void onGPRSStateChange(int state)
{
    switch (state)
    {
    case NW_STAT_NOT_REGISTERED:
        break;
    case NW_STAT_REGISTERED_HOME:
        break;
    case NW_STAT_SEARCHING:
        break;
    case NW_STAT_REG_DENIED:
        break;
    case NW_STAT_UNKNOWN:
        break;
    case NW_STAT_REGISTERED_ROAMING:
        break;
    case NW_STAT_SMSONLY_HOME:
        break;
    case NW_STAT_SMSONLY_ROAMING:
        break;
    default:
        break;
    }
}

void setup()
{
    Serial.begin(115200);
    /* let modem init */
    delay(5000);
    /* print information */
    Serial.print("IMEI: ");
    Serial.println(Modem.getIMEI());
    Serial.print("ICCID: ");
    Serial.println(Modem.getICCID());
    Serial.print("IMSI: ");
    Serial.println(Modem.getIMSI());
}

void loop()
{
    struct md_cellinfo_t info;

	delay(10000);
    /* LBS scanning @10s */
    Modem.getLBSInformation(&info);
    printf("MCC: %d, MNC: %d, LAC: %x, CellID: %x\n", info.cell.mcc, info.cell.mnc, info.cell.lac, info.cell.cellid);
}
