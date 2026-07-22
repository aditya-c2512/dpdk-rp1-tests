//
// Created by Aditya Choubey on 21/07/2026.
//
#include "tcp_client.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>

#include <netinet/tcp.h>
#include <sys/socket.h>


static void configure_socket(int fd)
{
    int value = 1;


    setsockopt(
        fd,
        IPPROTO_TCP,
        TCP_NODELAY,
        &value,
        sizeof(value));


    int size = 16 * 1024 * 1024;


    setsockopt(
        fd,
        SOL_SOCKET,
        SO_SNDBUF,
        &size,
        sizeof(size));
}



int tcp_client_connect(
        const char *address,
        uint16_t port)
{
    int fd;


    fd = socket(
            AF_INET,
            SOCK_STREAM,
            0);


    if(fd < 0)
        return -1;


    struct sockaddr_in addr;

    memset(
        &addr,
        0,
        sizeof(addr));


    addr.sin_family = AF_INET;

    addr.sin_port =
        htons(port);


    inet_pton(
        AF_INET,
        address,
        &addr.sin_addr);



    if(connect(
            fd,
            (struct sockaddr *)&addr,
            sizeof(addr)) < 0)
    {
        close(fd);
        return -1;
    }


    configure_socket(fd);


    return fd;
}
