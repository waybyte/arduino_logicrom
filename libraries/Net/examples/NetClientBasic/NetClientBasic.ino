/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <Net.h>

HardwareSerial serial(DEFAULT_STDIO_PORT);

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
  const uint16_t port = 80;
  const char *host = "google.com"; // ip or dns

  serial.print("Connecting to ");
  serial.println(host);

  // Use NetClient class to create TCP connections
  NetClient client;

  if (!client.connect(host, port))
  {
    serial.println("Connection failed.");
    serial.println("Waiting 5 seconds before retrying...");
    delay(5000);
    return;
  }

  // This will send a request to the server
  // uncomment this line to send an arbitrary string to the server
  // client.print("Send this data to the server");
  // uncomment this line to send a basic document request to the server
  client.print("GET /index.html HTTP/1.1\n\n");

  int maxloops = 0;

  // wait for the server's reply to become available
  while (!client.available() && maxloops < 1000)
  {
    maxloops++;
    delay(1); // delay 1 msec
  }
  if (client.available() > 0)
  {
    // read back one line from the server
    String line = client.readStringUntil('\r');
    serial.println(line);
  }
  else
  {
    serial.println("client.available() timed out ");
  }

  serial.println("Closing connection.");
  client.stop();

  serial.println("Waiting 5 seconds before restarting...");
  delay(5000);
}
