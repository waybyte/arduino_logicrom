#ifndef __NET_H__
#define __NET_H__

#include <Arduino.h>
#include <NetClient.h>
#include <NetClientSecure.h>
#include <NetUdp.h>

class NetworkClass
{
private:
	/* data */
	uint32_t _timeout;
public:
	NetworkClass();
	~NetworkClass();

	bool begin(bool data_en, uint32_t timeout, pin_size_t netled);
	bool begin(bool data_en, uint32_t timeout);
	bool begin(pin_size_t netled);
	bool begin();

	/* System Information */
	bool waitForRegistration(int timeout = 60000);
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
	bool GprsWaitForActivation(int timeout = 60000);
	bool isGprsActive(void);
	bool isDataEnable(void);
	int DataEnable(void);
	int DataDisable(void);
	bool waitDataReady(int timeout = 60000);
	bool isDataReady(void);

	/* Domain name resolve */
	IPAddress resolve(const char *domain);
};

extern NetworkClass Net;

#endif /* __NET_H__ */

