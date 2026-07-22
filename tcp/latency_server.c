//
// Created by Aditya Choubey on 21/07/2026.
//

#include "benchmark.h"

#include "latency.h"

#include "tcp_server.h"
#include "tcp_client.h"

#include "logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <inttypes.h>

int run_latency_server(
        int fd)
{
    latency_packet packet;


    while(1)
    {
        if(tcp_recv_all(
                fd,
                &packet,
                sizeof(packet)) < 0)
        {
            log_info(
                "Client disconnected");

            break;
        }



        if(packet.magic != LATENCY_MAGIC)
        {
            log_error(
                "Invalid packet magic 0x%x",
                packet.magic);

            continue;
        }



        if(tcp_send_all(
                fd,
                &packet,
                sizeof(packet)) < 0)
        {
            break;
        }
    }


    return 0;
}

int main(int argc,char **argv)
{
    if(argc < 2)
    {
        printf(
        "usage: %s <port>\n",
        argv[0]);

        return 1;
    }



    int server =
        tcp_server_listen(
            atoi(argv[1]));



    if(server < 0)
    {
        log_error(
            "Failed to listen");

        return 1;
    }



    log_info(
        "Waiting for latency client");



    int fd =
        tcp_server_accept(server);



    log_info(
        "Client connected");


    run_latency_server(fd);



    close(fd);
    close(server);


    return 0;
}