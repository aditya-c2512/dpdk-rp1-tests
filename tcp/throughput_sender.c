//
// Created by Aditya Choubey on 21/07/2026.
//

#include "benchmark.h"

#include "tcp_client.h"
#include "throughput.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if(argc < 5)
    {
        printf(
            "usage: %s <ip> <port> <size> <duration>\n",
            argv[0]);

        return 1;
    }


    int fd =
        tcp_client_connect(
            argv[1],
            atoi(argv[2]));


    if(fd < 0)
    {
        log_error("TCP connection failed");
        return 1;
    }


    benchmark_stats stats;

    stats_reset(&stats);



    uint32_t payload_size =
        atoi(argv[3]);

    uint32_t duration =
        atoi(argv[4]);



    run_sender_benchmark(
        fd,
        payload_size,
        duration,
        &stats);



    printf(
        "TX %.2f Mbps %.2f pps\n",
        stats.throughput_mbps,
        stats.packets_per_second);



    /*
     * Append results
     */
    csv_append(
        "../results/tx_throughput.csv",
        "TCP",
        payload_size,
        duration,
        &stats);



    close(fd);

    return 0;
}