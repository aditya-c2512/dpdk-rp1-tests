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

int main(int argc,char **argv)
{
    if(argc != 4)
    {
        printf(
            "usage: %s <port> <payload_size> <duration>\n",
            argv[0]);

        return 1;
    }



    uint16_t port =
        atoi(argv[1]);

    uint32_t payload_size =
        atoi(argv[2]);

    uint32_t duration =
        atoi(argv[3]);



    int fd =
        udp_server_bind(port);

    if(fd < 0)
    {
        log_error(
            "Failed to bind UDP socket");

        return 1;
    }



    benchmark_stats stats = {0};

    run_udp_receiver_benchmark(
        fd,
        payload_size,
        duration,
        &stats);



    printf(
        "RX %.2f Mbps %.2f pps\n",
        stats.throughput_mbps,
        stats.packets_per_second);



    throughput_csv_append(
        "../results/udp_rx_throughput.csv",
        "UDP",
        payload_size,
        duration,
        &stats);



    udp_close(fd);

    return 0;
}