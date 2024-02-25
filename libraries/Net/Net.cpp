#include <lib.h>
#include <network.h>
#include <arpa/inet.h>
#include <modem.h>
#include <netdb.h>
#include <utils.h>

#include "Net.h"

NetworkClass Net;

NetworkClass::NetworkClass():_timeout(60000) {}
NetworkClass::~NetworkClass() {}

bool NetworkClass::begin(bool data_en, uint32_t timeout, pin_size_t netled)
{
	if (data_en)
		DataEnable();
	else
		DataDisable();
	
	if (netled < GPIO_PIN_MAX)
		network_setup_statusled(netled);

	_timeout = timeout;
	
	return data_en ? waitDataReady(_timeout) : waitForRegistration();
}

bool NetworkClass::begin(bool data_en, uint32_t timeout)
{
	_timeout = timeout;
	return begin(data_en, _timeout, GPIO_PIN_MAX);
}

bool NetworkClass::begin(pin_size_t netled)
{
	return begin(true, _timeout, netled);
}

bool NetworkClass::begin(void)
{
	return begin(true, _timeout, GPIO_PIN_MAX);
}

bool NetworkClass::waitForRegistration(int timeout)
{
	while (timeout > 0) {
		if (network_getstatus(0) >= NET_STATE_GSM)
			return true;
		msleep(100);
		timeout -= 100;
	}

	return false;
}

bool NetworkClass::isRegistered(void)
{
	return network_getstatus(0) >= NET_STATE_GSM;
}

int NetworkClass::getSignalStrength(void)
{
	struct netparam_t netp;

	network_getparam(&netp);

	return netp.signal;
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
	return md_get_operatorname((char *)NULL, 0);
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
	return network_dataenable(true);
}

int NetworkClass::GprsDisable(void)
{
	return network_dataenable(false);
}

bool NetworkClass::isGprsEnable(void)
{
	return network_isdataenable() ? true : false;
}

bool NetworkClass::GprsWaitForActivation(int timeout)
{
	while (timeout > 0) {
		if (network_isdataready())
			return true;
		msleep(100);
		timeout -= 100;
	}

	return false;
}

bool NetworkClass::isGprsActive(void)
{
	return network_isdataready();
}

bool NetworkClass::isDataEnable(void)
{
	return isGprsEnable();
}

int NetworkClass::DataEnable(void)
{
	return GprsEnable();
}

int NetworkClass::DataDisable(void)
{
	return GprsDisable();
}

bool NetworkClass::waitDataReady(int timeout)
{
	return GprsWaitForActivation(timeout);
}

bool NetworkClass::isDataReady(void)
{
	return network_isdataready();
}

IPAddress NetworkClass::resolve(const char *domain)
{
	struct hostent *h = gethostbyname(domain);

	if (h)
		return IPAddress(((struct in_addr *)h->h_addr)->s_addr);
	else
		return IPAddress(0, 0, 0, 0);
}
