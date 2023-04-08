/*
  Client.h - Client class for Raspberry Pi
  Copyright (c) 2016 Hristo Gochkov  All right reserved.

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

#include "Net.h"
#include "NetClient.h"
#include <fcntl.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <errno.h>

#define NET_CLIENT_DEF_CONN_TIMEOUT_MS (10000)
#define NET_CLIENT_MAX_WRITE_RETRY (10)
#define NET_CLIENT_SELECT_TIMEOUT_US (1000000)
#define NET_CLIENT_FLUSH_BUFFER_SIZE (1024)

#undef connect
#undef write
#undef read

class NetClientRxBuffer
{
private:
    size_t _size;
    uint8_t *_buffer;
    size_t _pos;
    size_t _fill;
    int _fd;
    bool _failed;

    size_t r_available()
    {
        if (_fd < 0)
        {
            return 0;
        }
        int count;
        int res = ioctl(_fd, FIONREAD, &count);
        if (res < 0)
        {
            _failed = true;
            return 0;
        }
        return count;
    }

    size_t fillBuffer()
    {
        if (!_buffer)
        {
            _buffer = (uint8_t *)malloc(_size);
            if (!_buffer)
            {
                debug(DBG_OFF, "Not enough memory to allocate buffer");
                _failed = true;
                return 0;
            }
        }
        if (_fill && _pos == _fill)
        {
            _fill = 0;
            _pos = 0;
        }
        if (!_buffer || _size <= _fill || !r_available())
        {
            return 0;
        }
        int res = recv(_fd, _buffer + _fill, _size - _fill, MSG_DONTWAIT);
        if (res < 0)
        {
            if (errno != EWOULDBLOCK)
            {
                _failed = true;
            }
            return 0;
        }
        _fill += res;
        return res;
    }

public:
    NetClientRxBuffer(int fd, size_t size = 1436)
        : _size(size), _buffer(NULL), _pos(0), _fill(0), _fd(fd), _failed(false)
    {
        //_buffer = (uint8_t *)malloc(_size);
    }

    ~NetClientRxBuffer()
    {
        free(_buffer);
    }

    bool failed()
    {
        return _failed;
    }

    int read(uint8_t *dst, size_t len)
    {
        if (!dst || !len || (_pos == _fill && !fillBuffer()))
        {
            return _failed ? -1 : 0;
        }
        size_t a = _fill - _pos;
        if (len <= a || ((len - a) <= (_size - _fill) && fillBuffer() >= (len - a)))
        {
            if (len == 1)
            {
                *dst = _buffer[_pos];
            }
            else
            {
                memcpy(dst, _buffer + _pos, len);
            }
            _pos += len;
            return len;
        }
        size_t left = len;
        size_t toRead = a;
        uint8_t *buf = dst;
        memcpy(buf, _buffer + _pos, toRead);
        _pos += toRead;
        left -= toRead;
        buf += toRead;
        while (left)
        {
            if (!fillBuffer())
            {
                return len - left;
            }
            a = _fill - _pos;
            toRead = (a > left) ? left : a;
            memcpy(buf, _buffer + _pos, toRead);
            _pos += toRead;
            left -= toRead;
            buf += toRead;
        }
        return len;
    }

    int peek()
    {
        if (_pos == _fill && !fillBuffer())
        {
            return -1;
        }
        return _buffer[_pos];
    }

    size_t available()
    {
        return _fill - _pos + r_available();
    }
};

class NetClientSocketHandle
{
private:
    int sockfd;

public:
    NetClientSocketHandle(int fd) : sockfd(fd)
    {
    }

    ~NetClientSocketHandle()
    {
        close(sockfd);
    }

    int fd()
    {
        return sockfd;
    }
};

NetClient::NetClient() : _connected(false), _timeout(NET_CLIENT_DEF_CONN_TIMEOUT_MS), next(NULL)
{
}

NetClient::NetClient(int fd) : _connected(true), _timeout(NET_CLIENT_DEF_CONN_TIMEOUT_MS), next(NULL)
{
#ifndef SOC_RDA8955
    clientSocketHandle.reset(new NetClientSocketHandle(fd));
    _rxBuffer.reset(new NetClientRxBuffer(fd));
#else
    clientSocketHandle = new NetClientSocketHandle(fd);
    _rxBuffer = new NetClientRxBuffer(fd);
#endif
}

NetClient::~NetClient()
{
#ifdef SOC_RDA8955
    delete clientSocketHandle;
    delete _rxBuffer;
#endif
    stop();
}

NetClient &NetClient::operator=(const NetClient &other)
{
    stop();
    clientSocketHandle = other.clientSocketHandle;
    _rxBuffer = other._rxBuffer;
    _connected = other._connected;
    return *this;
}

void NetClient::stop()
{
    clientSocketHandle = NULL;
    _rxBuffer = NULL;
    _connected = false;
}

int NetClient::connect(IPAddress ip, uint16_t port)
{
    return connect(ip, port, _timeout);
}
int NetClient::connect(IPAddress ip, uint16_t port, int32_t timeout_ms)
{
    _timeout = timeout_ms;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        debug(DBG_OFF, "socket: %d", errno);
        return 0;
    }
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);

    remote_ip = ip;
    remote_port = port;
    uint32_t ip_addr = ip;
    struct sockaddr_in serveraddr;
    memset((char *)&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    memcpy((void *)&serveraddr.sin_addr.s_addr, (const void *)(&ip_addr), 4);
    serveraddr.sin_port = htons(port);
    fd_set fdset;
    struct timeval tv;
    FD_ZERO(&fdset);
    FD_SET(sockfd, &fdset);
    tv.tv_sec = _timeout / 1000;
    tv.tv_usec = (_timeout % 1000) * 1000;

    int res = ::connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (res < 0 && errno != EINPROGRESS)
    {
        debug(DBG_OFF, "connect on fd %d, errno: %d, \"%s\"", sockfd, errno, strerror(errno));
        close(sockfd);
        return 0;
    }

#if !defined(SOC_MT2503) && !defined(SOC_MT6261)
    res = ::select(sockfd + 1, NULL, &fdset, NULL, _timeout < 0 ? NULL : &tv);
    if (res < 0)
    {
        debug(DBG_OFF, "select on fd %d, errno: %d, \"%s\"", sockfd, errno, strerror(errno));
        close(sockfd);
        return 0;
    }
    else if (res == 0)
    {
        debug(DBG_INFO, "select returned due to timeout %d ms for fd %d", _timeout, sockfd);
        close(sockfd);
        return 0;
    }
    else
    {
#if 0 /* TODO */
        int sockerr;
        socklen_t len = (socklen_t)sizeof(int);
        res = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &sockerr, &len);

        if (res < 0)
        {
            debug(DBG_OFF, "getsockopt on fd %d, errno: %d, \"%s\"", sockfd, errno, strerror(errno));
            close(sockfd);
            return 0;
        }

        if (sockerr != 0)
        {
            debug(DBG_OFF, "socket error on fd %d, errno: %d, \"%s\"", sockfd, sockerr, strerror(sockerr));
            close(sockfd);
            return 0;
        }
#endif
    }
#endif

#define ROE_NetClient(x, msg)                                                                                           \
    {                                                                                                                   \
        if (((x) < 0))                                                                                                  \
        {                                                                                                               \
            debug(DBG_OFF, "Setsockopt '" msg "'' on fd %d failed. errno: %d, \"%s\"", sockfd, errno, strerror(errno)); \
            return 0;                                                                                                   \
        }                                                                                                               \
    }
    ROE_NetClient(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)), "SO_SNDTIMEO");
    ROE_NetClient(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)), "SO_RCVTIMEO");

    // These are also set in NetClientSecure, should be set here too?
    // ROE_NetClient(setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable)),"TCP_NODELAY");
    // ROE_NetClient (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable)),"SO_KEEPALIVE");

    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) & (~O_NONBLOCK));
#ifndef SOC_RDA8955
    clientSocketHandle.reset(new NetClientSocketHandle(sockfd));
    _rxBuffer.reset(new NetClientRxBuffer(sockfd));
#else
    clientSocketHandle = new NetClientSocketHandle(sockfd);
    _rxBuffer = new NetClientRxBuffer(sockfd);
#endif

    _connected = true;
    return 1;
}

int NetClient::connect(const char *host, uint16_t port)
{
    return connect(host, port, _timeout);
}

int NetClient::connect(const char *host, uint16_t port, int32_t timeout_ms)
{
    IPAddress srv = Net.resolve(host);
    if (srv == IPADDR_NONE)
    {
        return 0;
    }
    return connect(srv, port, timeout_ms);
}

int NetClient::setSocketOption(int option, char *value, size_t len)
{
    return setSocketOption(SOL_SOCKET, option, (const void *)value, len);
}

int NetClient::setSocketOption(int level, int option, const void *value, size_t len)
{
    int res = setsockopt(fd(), level, option, value, len);
    if (res < 0)
    {
        debug(DBG_OFF, "fail on %d, errno: %d, \"%s\"", fd(), errno, strerror(errno));
    }
    return res;
}

int NetClient::setTimeout(uint32_t seconds)
{
    Client::setTimeout(seconds * 1000); // This should be here?
    _timeout = seconds * 1000;
    if (fd() >= 0)
    {
        struct timeval tv;
        tv.tv_sec = seconds;
        tv.tv_usec = 0;
        if (setSocketOption(SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)) < 0)
        {
            return -1;
        }
        return setSocketOption(SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
    }
    else
    {
        return 0;
    }
}

int NetClient::setOption(int option, int *value)
{
    return setSocketOption(IPPROTO_TCP, option, (const void *)value, sizeof(int));
}

int NetClient::getOption(int option, int *value)
{
    socklen_t size = sizeof(int);
    int res = getsockopt(fd(), IPPROTO_TCP, option, (char *)value, &size);
    if (res < 0)
    {
        debug(DBG_OFF, "fail on fd %d, errno: %d, \"%s\"", fd(), errno, strerror(errno));
    }
    return res;
}

int NetClient::setNoDelay(bool nodelay)
{
    int flag = nodelay;
    return setOption(TCP_NODELAY, &flag);
}

bool NetClient::getNoDelay()
{
    int flag = 0;
    getOption(TCP_NODELAY, &flag);
    return flag;
}

size_t NetClient::write(uint8_t data)
{
    return write(&data, 1);
}

int NetClient::read()
{
    uint8_t data = 0;
    int res = read(&data, 1);
    if (res < 0)
    {
        return res;
    }
    if (res == 0)
    { //  No data available.
        return -1;
    }
    return data;
}

size_t NetClient::write(const uint8_t *buf, size_t size)
{
    int res = 0;
    int retry = NET_CLIENT_MAX_WRITE_RETRY;
    int socketFileDescriptor = fd();
    size_t totalBytesSent = 0;
    size_t bytesRemaining = size;

    if (!_connected || (socketFileDescriptor < 0))
    {
        return 0;
    }

    while (retry)
    {
#if !defined(SOC_MT2503) && !defined(SOC_MT6261)
        // use select to make sure the socket is ready for writing
        fd_set set;
        struct timeval tv;
        FD_ZERO(&set);                      // empties the set
        FD_SET(socketFileDescriptor, &set); // adds FD to the set
        tv.tv_sec = 0;
        tv.tv_usec = NET_CLIENT_SELECT_TIMEOUT_US;
        retry--;

        if (select(socketFileDescriptor + 1, NULL, &set, NULL, &tv) < 0)
        {
            return 0;
        }

        if (FD_ISSET(socketFileDescriptor, &set))
        {
            res = send(socketFileDescriptor, (void *)buf, bytesRemaining, MSG_DONTWAIT);
            if (res > 0)
            {
                totalBytesSent += res;
                if (totalBytesSent >= size)
                {
                    // completed successfully
                    retry = 0;
                }
                else
                {
                    buf += res;
                    bytesRemaining -= res;
                    retry = NET_CLIENT_MAX_WRITE_RETRY;
                }
            }
            else if (res < 0)
            {
                debug(DBG_OFF, "fail on fd %d, errno: %d, \"%s\"", fd(), errno, strerror(errno));
                if (errno != EAGAIN)
                {
                    // if resource was busy, can try again, otherwise give up
                    stop();
                    res = 0;
                    retry = 0;
                }
            }
            else
            {
                // Try again
            }
        }
#else
        retry--;
        res = send(socketFileDescriptor, (void *)buf, bytesRemaining, MSG_DONTWAIT);
        if (res > 0)
        {
            totalBytesSent += res;
            if (totalBytesSent >= size)
            {
                // completed successfully
                retry = 0;
            }
            else
            {
                buf += res;
                bytesRemaining -= res;
                retry = NET_CLIENT_MAX_WRITE_RETRY;
            }
        }
        else if (res < 0)
        {
            debug(DBG_OFF, "fail on fd %d, errno: %d, \"%s\"", fd(), errno, strerror(errno));
            if (errno != EAGAIN)
            {
                // if resource was busy, can try again, otherwise give up
                stop();
                res = 0;
                retry = 0;
            }
        }
        else
        {
            // Try again
        }
#endif
    }
    return totalBytesSent;
}

size_t NetClient::write_P(PGM_P buf, size_t size)
{
    return write(buf, size);
}

size_t NetClient::write(Stream &stream)
{
    uint8_t *buf = (uint8_t *)malloc(1360);
    if (!buf)
    {
        return 0;
    }
    size_t toRead = 0, toWrite = 0, written = 0;
    size_t available = stream.available();
    while (available)
    {
        toRead = (available > 1360) ? 1360 : available;
        toWrite = stream.readBytes(buf, toRead);
        written += write(buf, toWrite);
        available = stream.available();
    }
    free(buf);
    return written;
}

int NetClient::read(uint8_t *buf, size_t size)
{
    int res = -1;
    if (_rxBuffer)
    {
        res = _rxBuffer->read(buf, size);
        if (_rxBuffer->failed())
        {
            debug(DBG_OFF, "fail on fd %d, errno: %d, \"%s\"", fd(), errno, strerror(errno));
            stop();
        }
    }
    return res;
}

int NetClient::peek()
{
    int res = -1;
    if (_rxBuffer)
    {
        res = _rxBuffer->peek();
        if (_rxBuffer->failed())
        {
            debug(DBG_OFF, "fail on fd %d, errno: %d, \"%s\"", fd(), errno, strerror(errno));
            stop();
        }
    }
    return res;
}

int NetClient::available()
{
    if (!_rxBuffer)
    {
        return 0;
    }
    int res = _rxBuffer->available();
    if (_rxBuffer->failed())
    {
        debug(DBG_OFF, "fail on fd %d, errno: %d, \"%s\"", fd(), errno, strerror(errno));
        stop();
    }
    return res;
}

// Though flushing means to send all pending data,
// seems that in Arduino it also means to clear RX
void NetClient::flush()
{
    int res;
    size_t a = available(), toRead = 0;
    if (!a)
    {
        return; // nothing to flush
    }
    uint8_t *buf = (uint8_t *)malloc(NET_CLIENT_FLUSH_BUFFER_SIZE);
    if (!buf)
    {
        return; // memory error
    }
    while (a)
    {
        toRead = (a > NET_CLIENT_FLUSH_BUFFER_SIZE) ? NET_CLIENT_FLUSH_BUFFER_SIZE : a;
        res = recv(fd(), buf, toRead, MSG_DONTWAIT);
        if (res < 0)
        {
            debug(DBG_OFF, "fail on fd %d, errno: %d, \"%s\"", fd(), errno, strerror(errno));
            stop();
            break;
        }
        a -= res;
    }
    free(buf);
}

uint8_t NetClient::connected()
{
    if (_connected)
    {
        uint8_t dummy;
        int res = recv(fd(), &dummy, 0, MSG_DONTWAIT);
        // avoid unused var warning by gcc
        (void)res;
        // recv only sets errno if res is <= 0
        if (res <= 0)
        {
            switch (errno)
            {
            case EWOULDBLOCK:
            case ENOENT: // caused by vfs
                _connected = true;
                break;
            case ENOTCONN:
            case EPIPE:
            case ECONNRESET:
            case ECONNREFUSED:
            case ECONNABORTED:
                _connected = false;
                debug(DBG_VERBOSE, "Disconnected: RES: %d, ERR: %d", res, errno);
                break;
            default:
                debug(DBG_INFO, "Unexpected: RES: %d, ERR: %d", res, errno);
                _connected = true;
                break;
            }
        }
        else
        {
            _connected = true;
        }
    }
    return _connected;
}

IPAddress NetClient::remoteIP(int fd) const
{
    return remote_ip;
}

uint16_t NetClient::remotePort(int fd) const
{
    return remote_port;
}

IPAddress NetClient::remoteIP() const
{
    return remoteIP(fd());
}

uint16_t NetClient::remotePort() const
{
    return remotePort(fd());
}

bool NetClient::operator==(const NetClient &rhs)
{
    return clientSocketHandle == rhs.clientSocketHandle && remotePort() == rhs.remotePort() && remoteIP() == rhs.remoteIP();
}

int NetClient::fd() const
{
    if (clientSocketHandle == NULL)
    {
        return -1;
    }
    else
    {
        return clientSocketHandle->fd();
    }
}
