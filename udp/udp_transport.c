//
// Created by Aditya Choubey on 23/07/2026.
//
#include "udp_transport.h"

#include "logging.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

int udp_client_connect(
        const char *ip,
        uint16_t port)
{
    int fd =
        socket(AF_INET, SOCK_DGRAM, 0);

    if(fd < 0)
    {
        log_error("socket()");
        return -1;
    }

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if(inet_pton(AF_INET, ip, &addr.sin_addr) != 1)
    {
        log_error("Invalid IP");

        close(fd);

        return -1;
    }

    if(connect(fd,
               (struct sockaddr *)&addr,
               sizeof(addr)) < 0)
    {
        log_error("connect()");

        close(fd);

        return -1;
    }

    return fd;
}

int udp_server_bind(
        uint16_t port)
{
    int fd =
        socket(AF_INET, SOCK_DGRAM, 0);

    if(fd < 0)
    {
        log_error("socket()");
        return -1;
    }

    int one = 1;

    setsockopt(fd,
               SOL_SOCKET,
               SO_REUSEADDR,
               &one,
               sizeof(one));

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(fd,
            (struct sockaddr *)&addr,
            sizeof(addr)) < 0)
    {
        log_error("bind()");

        close(fd);

        return -1;
    }

    return fd;
}

ssize_t udp_send(
        int fd,
        const void *buf,
        size_t len)
{
    ssize_t ret =
        send(fd, buf, len, 0);

    if(ret < 0)
    {
        log_error(
            "send failed errno=%d (%s)",
            errno,
            strerror(errno));
    }

    return ret;
}

ssize_t udp_recv(
        int fd,
        void *buf,
        size_t len)
{
    ssize_t ret =
        recv(fd,
             buf,
             len,
             0);

    if(ret < 0)
        log_error("recv()");

    return ret;
}

ssize_t udp_recv_from(
        int fd,
        void *buf,
        size_t len,
        struct sockaddr_in *peer,
        socklen_t *peer_len)
{
    ssize_t ret =
        recvfrom(
            fd,
            buf,
            len,
            0,
            (struct sockaddr *)peer,
            peer_len);

    if(ret < 0)
        log_error("recvfrom()");

    return ret;
}

ssize_t udp_send_to(
        int fd,
        const void *buf,
        size_t len,
        const struct sockaddr_in *peer,
        socklen_t peer_len)
{
    ssize_t ret =
        sendto(
            fd,
            buf,
            len,
            0,
            (const struct sockaddr *)peer,
            peer_len);

    if(ret < 0)
    {
        log_error(
            "sendto failed errno=%d (%s)",
            errno,
            strerror(errno));
    }

    return ret;
}

void udp_close(
        int fd)
{
    close(fd);
}
