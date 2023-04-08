/*
  NetClientSecure.cpp - Client Secure class for ESP32
  Copyright (c) 2016 Hristo Gochkov  All right reserved.
  Additions Copyright (C) 2017 Evandro Luis Copercini.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "NetClientSecure.h"
#include <arpa/inet.h>
#include <errno.h>

NetClientSecure::NetClientSecure()
{
    _connected = false;
    _timeout = 30000;
    ssl_fd = -1;
    _peek = -1;
    _lastError = 0;
    memset(&_opts, 0, sizeof(_opts));
    memset(&_certs, 0, sizeof(_certs));
}

NetClientSecure::~NetClientSecure()
{
    stop();
}

NetClientSecure &NetClientSecure::operator=(const NetClientSecure &other)
{
    stop();
    ssl_fd = other.ssl_fd;
    _connected = other._connected;
    _opts = other._opts;
    _certs = other._certs;
    return *this;
}

void NetClientSecure::stop()
{
    if (ssl_fd > 0)
    {
        ssl_socket_free(ssl_fd);
        ssl_fd = -1;
        _connected = false;
        _peek = -1;
        memset(&_opts, 0, sizeof(_opts));
        memset(&_certs, 0, sizeof(_certs));
    }
}

int NetClientSecure::connect(IPAddress ip, uint16_t port)
{
    return connect(ip, port, NULL, NULL, NULL);
}

int NetClientSecure::connect(IPAddress ip, uint16_t port, int32_t timeout)
{
    _timeout = timeout;
    return connect(ip, port);
}

int NetClientSecure::connect(const char *host, uint16_t port)
{
    return connect(host, port, NULL, NULL, NULL);
}

int NetClientSecure::connect(const char *host, uint16_t port, int32_t timeout)
{
    _timeout = timeout;
    return connect(host, port);
}

int NetClientSecure::connect(IPAddress ip, uint16_t port, const char *CA_cert, const char *cert, const char *private_key)
{
    return connect(ip, port, NULL, CA_cert, cert, private_key);
}

int NetClientSecure::connect(const char *host, uint16_t port, const char *CA_cert, const char *cert, const char *private_key)
{
    return connect(IPADDR_NONE, port, host, CA_cert, cert, private_key);
}

int NetClientSecure::connect(IPAddress ip, uint16_t port, const char *host, const char *CA_cert, const char *cert, const char *private_key)
{
    int ret;
    in_addr ipaddr;

    if (host)
        strcpy(_opts.server_ip, host);
    else if (ip != IPADDR_NONE)
    {
        ipaddr.s_addr = (uint32_t)ip;
        strcpy(_opts.server_ip, inet_ntoa(ipaddr));
    }
    else
        return 0;

    _opts.port = port;
    _opts.arg = this;

    /* setup Root CA */
    if (CA_cert)
    {
        _certs.rootca = CA_cert;
        _certs.rootca_len = strlen(CA_cert) + 1;
    }

    /* setup certs */
    if (cert && private_key)
    {
        /* client certificate */
        _certs.cert = cert;
        _certs.cert_len = strlen(cert) + 1;
        /* private key */
        _certs.privatekey = private_key;
        _certs.privatekey_len = strlen(private_key) + 1;
    }

    ssl_fd = ssl_socket_request(&_certs);
    if (ssl_fd < 0)
    {
        debug(DBG_OFF, "SSL request err: %d\n", ssl_fd);
        return 0;
    }

    ret = ssl_socket_setopt(ssl_fd, &_opts);
    if (ret < 0)
    {
        debug(DBG_OFF, "SSL config err: %d\n", ret);
        return 0;
    }

    ret = ssl_socket_open(ssl_fd);
    if (ret < 0)
    {
        debug(DBG_OFF, "SSL conn err: %d\n", ret);
        return 0;
    }
    _connected = true;
    return 1;
}

int NetClientSecure::peek()
{
    if (_peek >= 0)
    {
        return _peek;
    }
    _peek = timedRead();
    return _peek;
}

size_t NetClientSecure::write(uint8_t data)
{
    return write(&data, 1);
}

int NetClientSecure::read()
{
    uint8_t data = -1;
    int res = read(&data, 1);
    if (res < 0)
    {
        return res;
    }
    return data;
}

size_t NetClientSecure::write(const uint8_t *buf, size_t size)
{
    if (!_connected)
    {
        return 0;
    }
    int res = ssl_socket_send(ssl_fd, buf, size, _timeout / 1000);
    if (res < 0)
    {
        stop();
        res = 0;
    }
    return res;
}

int NetClientSecure::read(uint8_t *buf, size_t size)
{
    int peeked = 0;
    if (!buf && size)
    {
        return -1;
    }
    if (!size)
    {
        return 0;
    }
    if (_peek >= 0)
    {
        buf[0] = _peek;
        _peek = -1;
        size--;
        if (!size)
        {
            return 1;
        }
        buf++;
        peeked = 1;
    }

    /* only support timed read */
    int res = ssl_socket_read(ssl_fd, buf, size, _timeout / 1000);
    if (res < 0)
    {
        stop();
        return peeked ? peeked : res;
    }
    return res + peeked;
}

int NetClientSecure::available()
{
    int peeked = (_peek >= 0);
    if (!_connected)
    {
        return peeked;
    }
    /* if peeked already */
    if (peeked)
        return peeked;
    /* TODO: only timed read available */
    /* peek 1 byte to check if available */
    peeked = peek() > 0;
    return peeked;
}

uint8_t NetClientSecure::connected()
{
    uint8_t dummy = 0;
    read(&dummy, 0);

    return _connected;
}

void NetClientSecure::setInsecure()
{
    memset(&_certs, 0, sizeof(_certs));
}

void NetClientSecure::setCACert(const char *rootCA)
{
    _certs.rootca = rootCA;
    _certs.rootca_len = strlen(rootCA) + 1;
}

void NetClientSecure::setCertificate(const char *client_ca)
{
    _certs.cert = client_ca;
    _certs.cert_len = strlen(client_ca) + 1;
}

void NetClientSecure::setPrivateKey(const char *private_key)
{
    _certs.privatekey = private_key;
    _certs.privatekey_len = strlen(private_key) + 1;
}

char *NetClientSecure::_streamLoad(Stream &stream, size_t size)
{
    char *dest = (char *)malloc(size + 1);
    if (!dest)
    {
        return NULL;
    }
    if (size != stream.readBytes(dest, size))
    {
        free(dest);
        dest = NULL;
        return NULL;
    }
    dest[size] = '\0';
    return dest;
}

bool NetClientSecure::loadCACert(Stream &stream, size_t size)
{
    if (_certs.rootca != NULL)
        free(const_cast<char *>(_certs.rootca));
    char *dest = _streamLoad(stream, size);
    bool ret = false;
    if (dest)
    {
        setCACert(dest);
        ret = true;
    }
    return ret;
}

bool NetClientSecure::loadCertificate(Stream &stream, size_t size)
{
    if (_certs.cert != NULL)
        free(const_cast<char *>(_certs.cert));
    char *dest = _streamLoad(stream, size);
    bool ret = false;
    if (dest)
    {
        setCertificate(dest);
        ret = true;
    }
    return ret;
}

bool NetClientSecure::loadPrivateKey(Stream &stream, size_t size)
{
    if (_certs.privatekey != NULL)
        free(const_cast<char *>(_certs.privatekey));
    char *dest = _streamLoad(stream, size);
    bool ret = false;
    if (dest)
    {
        setPrivateKey(dest);
        ret = true;
    }
    return ret;
}

int NetClientSecure::lastError(char *buf, const size_t size)
{
    if (!_lastError)
    {
        return 0;
    }
    snprintf(buf, size, "ssl error: %d", _lastError);
    return _lastError;
}

int NetClientSecure::setTimeout(uint32_t seconds)
{
    _timeout = seconds * 1000;
    return 0;
}

int NetClientSecure::fd() const
{
    return ssl_fd;
}
