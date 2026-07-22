//
// Created by Aditya Choubey on 21/07/2026.
//

#include "benchmark.h"

#include "tcp_client.h"
#include "throughput.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc,char **argv)
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



    benchmark_stats stats;

    stats_reset(&stats);



    run_sender_benchmark(
        fd,
        atoi(argv[3]),
        atoi(argv[4]),
        &stats);



    printf(
        "TX %.2f Mbps %.2f pps\n",
        stats.throughput_mbps,
        stats.packets_per_second);



    return 0;
}