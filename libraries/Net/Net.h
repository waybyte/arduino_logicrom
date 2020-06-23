#ifndef __NET_H__
#define __NET_H__

#include <IPAddress.h>

class NetworkClass
{
private:
	/* data */
public:
	NetworkClass();
	~NetworkClass();

	/* System Information */
	bool waitForRegistration(int timeout = 30000);
	bool isRegistered(void);
	int getSignalStrength(void);
	int getSimStatus(void);
	int getCreg(void);
	int getCgreg(void);
	const char *getOperator(void);

	/* APN Configuration */
	int setAPN(const char *apn_name, const char *username = NULL, const char *password = NULL);
	int getAPN(char *apn_name, char *username = NULL, char *password = NULL);

	/* GPRS API */
	int GprsEnable(void);
	int GprsDisable(void);
	bool isGprsEnable(void);
	bool GprsWaitForActivation(int timeout = 30000);
	bool isGprsActive(void);
	IPAddress localIP(void);

	/* system information */
	const char *getIMEI(void);
	const char *getIMSI(void);
	const char *getICCID(void);

	/* Domain name resolve */
	IPAddress resolve(const char *domain);
};

extern NetworkClass Net;

#endif /* __NET_H__ */

