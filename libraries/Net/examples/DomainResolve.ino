/*
 * DomainResolve.ino
 * This is an example to show how to use resolve method of Net library
 * 
 * If you have any questions, please post queries on github
 */

#include <Arduino.h>
#include <Net.h>

void setup()
{
	Net.waitForRegistration();

	/* Print Operator Name */
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
		printf("Device IP Address: %d.%d.%d.%d\n", devip[0], devip[1], devip[2], devip[3]);

		/* Resolve google.com domain */
		IPAddress ip = Net.resolve("google.com");
		if (ip != IPAddress(0, 0, 0, 0)) {
			printf("google.com IP Address: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
		} else {
			printf("Fail to resolve google.com\n");
		}
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
