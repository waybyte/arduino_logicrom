#include <lib.h>
#include <network.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <utils.h>

#include "Net.h"

NetworkClass Net;

NetworkClass::NetworkClass() {}
NetworkClass::~NetworkClass() {}

/* System Information */
bool NetworkClass::waitForRegistration(int timeout)
{
	while (timeout > 0) {
		if (network_getstatus(0) == NET_STATE_GSM)
			break;
		msleep(100);
		timeout -= 100;
	}

	return false;
}

bool NetworkClass::isRegistered(void)
{
	return network_getstatus(0) == NET_STATE_GSM;
}

int NetworkClass::getSignalStrength(void)
{
	struct netparam_t netp;

	network_getparam(&netp);

	return netp.csq;
}

int NetworkClass::getSimStatus(void)
{
	struct netparam_t netp;

	network_getparam(&netp);

	return netp.simstate;
}

int NetworkClass::getCreg(void)
{
	struct netparam_t netp;

	network_getparam(&netp);

	return netp.creg;
}

int NetworkClass::getCgreg(void)
{
	struct netparam_t netp;

	network_getparam(&netp);

	return netp.cgreg;
}

const char *NetworkClass::getOperator(void)
{
	return ::get_operatorname(NULL, 0);
}

/* APN Configuration */
int NetworkClass::setAPN(const char *apn_name, const char *username, const char *password)
{
	return network_setapn(apn_name, username, password);
}

int NetworkClass::getAPN(char *apn_name, char *username, char *password)
{
	return network_getapn(apn_name, username, password);
}

/* GPRS API */
int NetworkClass::GprsEnable(void)
{
	return network_gprsenable(true);
}

int NetworkClass::GprsDisable(void)
{
	return network_gprsenable(false);
}

bool NetworkClass::isGprsEnable(void)
{
	return network_isgprsenable() ? true : false;
}

bool NetworkClass::GprsWaitForActivation(int timeout)
{
	while (timeout > 0) {
		if (network_getstatus(0) == NET_STATE_GPRS)
			return true;
		msleep(100);
		timeout -= 100;
	}

	return false;
}

bool NetworkClass::isGprsActive(void)
{
	return network_getstatus(0) == NET_STATE_GPRS;
}

IPAddress NetworkClass::localIP(void)
{
	const uint8_t *ip = network_getlocalip();

	return IPAddress(ip[0], ip[1], ip[2], ip[3]);
}

const char *NetworkClass::getIMEI(void)
{
	return get_imei(NULL, 0);
}

const char *NetworkClass::getIMSI(void)
{
	return get_imsi(NULL, 0);
}

const char *NetworkClass::getICCID(void)
{
	return get_ccid(NULL, 0);
}

IPAddress NetworkClass::resolve(const char *domain)
{
	struct hostent *h = gethostbyname(domain);

	if (h)
		return IPAddress(((struct in_addr *)h->h_addr)->s_addr);
	else
		return IPAddress(0, 0, 0, 0);
}

