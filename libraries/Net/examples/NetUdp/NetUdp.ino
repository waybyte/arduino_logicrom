#include "Net.h"

NetUdp ntpClient;
HardwareSerial serial(DEFAULT_STDIO_PORT);

void dataConnectedLoop()
{
    // lets check the time
    const int NTP_PACKET_SIZE = 48;
    byte ntpPacketBuffer[NTP_PACKET_SIZE];

    IPAddress address = Net.resolve("time.nist.gov");
    memset(ntpPacketBuffer, 0, NTP_PACKET_SIZE);
    ntpPacketBuffer[0] = 0b11100011; // LI, Version, Mode
    ntpPacketBuffer[1] = 0;          // Stratum, or type of clock
    ntpPacketBuffer[2] = 6;          // Polling Interval
    ntpPacketBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    ntpPacketBuffer[12] = 49;
    ntpPacketBuffer[13] = 0x4E;
    ntpPacketBuffer[14] = 49;
    ntpPacketBuffer[15] = 52;
    ntpClient.beginPacket(address, 123); // NTP requests are to port 123
    ntpClient.write(ntpPacketBuffer, NTP_PACKET_SIZE);
    ntpClient.endPacket();

    delay(1000);

    int packetLength = ntpClient.parsePacket();
    if (packetLength)
    {
        if (packetLength >= NTP_PACKET_SIZE)
        {
            ntpClient.read(ntpPacketBuffer, NTP_PACKET_SIZE);
        }
        ntpClient.flush();
        uint32_t secsSince1900 = (uint32_t)ntpPacketBuffer[40] << 24 | (uint32_t)ntpPacketBuffer[41] << 16 | (uint32_t)ntpPacketBuffer[42] << 8 | ntpPacketBuffer[43];
        // serial.printf("Seconds since Jan 1 1900: %u\n", secsSince1900);
        uint32_t epoch = secsSince1900 - 2208988800UL;
        // serial.printf("EPOCH: %u\n", epoch);
        uint8_t h = (epoch % 86400L) / 3600;
        uint8_t m = (epoch % 3600) / 60;
        uint8_t s = (epoch % 60);
        char buf[50];
        sprintf(buf, "UTC: %02u:%02u:%02u (GMT)", h, m, s);
        serial.println(buf);
    }

    delay(9000);
}

void setup()
{
    int timeout = 60;
    serial.begin(115200);

    // Start Data connection

    serial.println();
    serial.println("******************************************************");
    if (Net.begin())
    {
        serial.println("Data connection ready!");
    }
    else
    {
        serial.println("Data connection not ready.");
    }

    serial.print("waiting");
    while (!Net.isDataReady())
    {
        if (timeout-- <= 0)
        {
            serial.println("Data connection failed! rebooting...");
            sys_reset();
        }
        delay(1000);
        serial.print(".");
    }
}

void loop()
{
    if (Net.isDataReady())
        dataConnectedLoop();

    while (serial.available())
        serial.write(serial.read());
}
