//
// Created by Aditya Choubey on 21/07/2026.
//
#include "tcp_server.h"

#include <unistd.h>
#include <string.h>


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
        SO_RCVBUF,
        &size,
        sizeof(size));
}




int tcp_server_listen(
        uint16_t port)
{
    int fd;


    fd = socket(
        AF_INET,
        SOCK_STREAM,
        0);


    if(fd < 0)
        return -1;


    int yes = 1;


    setsockopt(
        fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &yes,
        sizeof(yes));



    struct sockaddr_in addr;


    memset(
        &addr,
        0,
        sizeof(addr));


    addr.sin_family = AF_INET;

    addr.sin_addr.s_addr =
        INADDR_ANY;

    addr.sin_port =
        htons(port);



    if(bind(
        fd,
        (struct sockaddr *)&addr,
        sizeof(addr)) < 0)
    {
        close(fd);
        return -1;
    }



    if(listen(fd,1)<0)
    {
        close(fd);
        return -1;
    }


    return fd;
}




int tcp_server_accept(
        int server_fd)
{
    int fd;


    fd = accept(
        server_fd,
        NULL,
        NULL);


    if(fd>=0)
        configure_socket(fd);


    return fd;
}