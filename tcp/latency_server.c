//
// Created by Aditya Choubey on 21/07/2026.
//

#include "benchmark.h"

#include "latency.h"

#include "tcp_server.h"
#include "tcp_transport.h"

#include "logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <inttypes.h>
#include <string.h>

int run_latency_server(
        int fd)
{
    latency_header header;


    while(1)
    {
        /*
         * First receive the fixed-size header
         */
        if(tcp_recv_all(
                fd,
                &header,
                sizeof(header)) < 0)
        {
            log_info(
                "Client disconnected");

            break;
        }



        if(header.magic != LATENCY_MAGIC)
        {
            log_error(
                "Invalid packet magic 0x%x",
                header.magic);

            continue;
        }



        uint32_t packet_size =
            sizeof(latency_header)
            +
            header.payload_size;



        /*
         * Allocate complete packet
         *
         * +----------------+
         * | latency_header |
         * +----------------+
         * | payload        |
         * +----------------+
         */
        char *buffer =
            malloc(packet_size);



        if(!buffer)
        {
            log_error(
                "Failed to allocate packet");

            break;
        }



        /*
         * Copy already received header
         */
        memcpy(
            buffer,
            &header,
            sizeof(header));



        /*
         * Receive payload
         */
        if(header.payload_size > 0)
        {
            if(tcp_recv_all(
                    fd,
                    buffer + sizeof(header),
                    header.payload_size) < 0)
            {
                free(buffer);

                log_info(
                    "Client disconnected");

                break;
            }
        }



        /*
         * Echo complete packet back
         */
        if(tcp_send_all(
                fd,
                buffer,
                packet_size) < 0)
        {
            free(buffer);

            break;
        }



        free(buffer);
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


    while (1) {
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
    }



    return 0;
}