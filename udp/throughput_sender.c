//
// Created by Aditya Choubey on 23/07/2026.
//

#include "benchmark.h"

#include "throughput.h"
#include "udp_transport.h"

#include "csv.h"
#include "logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if(argc != 5)
    {
        printf(
            "usage: %s <ip> <port> <payload_size> <duration>\n",
            argv[0]);

        return 1;
    }

    const char *ip = argv[1];

    uint16_t port =
        atoi(argv[2]);

    uint32_t payload_size =
        atoi(argv[3]);

    uint32_t duration =
        atoi(argv[4]);



    int fd =
        udp_client_connect(
            ip,
            port);

    if(fd < 0)
    {
        log_error(
            "Failed to connect UDP socket");

        return 1;
    }



    benchmark_stats stats = {0};

    run_udp_sender_benchmark(
        fd,
        payload_size,
        duration,
        &stats);



    printf(
        "TX %.2f Mbps %.2f pps\n",
        stats.throughput_mbps,
        stats.packets_per_second);



    throughput_csv_append(
        "../results/udp_tx_throughput.csv",
        "UDP",
        payload_size,
        duration,
        &stats);



    udp_close(fd);

    return 0;
}