//
// Created by Aditya Choubey on 23/07/2026.
//

#include "benchmark.h"

#include "latency.h"
#include "udp_transport.h"
#include "logging.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf(
            "usage: %s <port>\n",
            argv[0]);

        return 1;
    }

    int fd =
        udp_server_bind(
            atoi(argv[1]));

    if(fd < 0)
    {
        log_error(
            "Failed to bind UDP socket");

        return 1;
    }

    log_info(
        "UDP latency server listening on port %s",
        argv[1]);

    run_udp_latency_server(fd);

    udp_close(fd);

    return 0;
}