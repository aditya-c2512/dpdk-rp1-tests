//
// Created by Aditya Choubey on 22/07/2026.
//

#include "tcp_transport.h"

#include <unistd.h>
#include <sys/socket.h>


int tcp_send_all(
        int fd,
        const void *buffer,
        uint32_t length)
{
    uint32_t sent = 0;


    while(sent < length)
    {
        int rc =
            send(
                fd,
                buffer + sent,
                length - sent,
                0);


        if(rc <= 0)
            return -1;


        sent += rc;
    }


    return sent;
}



int tcp_recv_all(
        int fd,
        void *buffer,
        uint32_t length)
{
    uint32_t received = 0;


    while(received < length)
    {
        int rc =
            recv(
                fd,
                buffer + received,
                length - received,
                0);


        if(rc <= 0)
            return -1;


        received += rc;
    }


    return received;
}