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
	/* Wait for data ready! */
	if (Net.begin())
		printf("Data connection ready!");
	else
		printf("Data connection failed!");

	/* Print Operator Name */
	printf("Operator Name: %s\n", Net.getOperator());

	/* Resolve if data connection ok */
	if (Net.isDataReady())
	{
		/* Resolve google.com domain */
		IPAddress ip = Net.resolve("google.com");
		if (ip != IPADDR_NONE)
		{
			printf("google.com IP Address: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
		}
		else
		{
			printf("Fail to resolve google.com\n");
		}
	}
}

void loop()
{
	/* Rest of code */
	delay(1000);
}
