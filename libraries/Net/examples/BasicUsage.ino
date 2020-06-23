/*
 * BasicUsage.ino
 * This is an example to show how to use network library
 * 
 * If you have any questions, please post queries on github
 */

#include <Arduino.h>
#include <Net.h>

void setup()
{
	/* Wait for network registration */
	Net.waitForRegistration();

	/* Print network status */
	printf("CSQ: %d\n", Net.getSignalStrength());
	printf("SIM Status: %d\n", Net.getSimStatus());
	printf("CREG: %d\n", Net.getCreg());
	printf("CGREG: %d\n", Net.getCgreg());
	printf("Operator Name: %s\n", Net.getOperator());

	/* Enable GPRS */
	Net.GprsEnable();
	/* Wait for GPRS Activation */
	Net.GprsWaitForActivation();

	/* Print GPRS status */
	if (Net.isGprsActive())
	{
		printf("GPRS Activated\n");
		IPAddress devip = Net.localIP();
		printf("IP Address: %d.%d.%d.%d\n", devip[0], devip[1], devip[2], devip[3]);
	}
	else
	{
		printf("GPRS Activation failed\n");
	}
}

void loop()
{
	/* Rest of code */
	delay(1000);
}