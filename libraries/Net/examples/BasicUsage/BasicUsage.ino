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
	/* Wait for data ready! */
	if (Net.begin())
		printf("Data connection ready!");
	else
		printf("Data connection failed!");

	/* Print network status */
	printf("CSQ: %d\n", Net.getSignalStrength());
	printf("SIM Status: %d\n", Net.getSimStatus());
	printf("CREG: %d\n", Net.getCreg());
	printf("CGREG: %d\n", Net.getCgreg());
	printf("Operator Name: %s\n", Net.getOperator());
}

void loop()
{
	/* Rest of code */
	delay(1000);
}