//
// Created by Aditya Choubey on 21/07/2026.
//

#include "benchmark.h"

#include "tcp_server.h"
#include "throughput.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc,char **argv)
{

    if(argc < 4)
    {
        printf(
        "usage: %s <port> <size> <duration>\n",
        argv[0]);

        return 1;
    }



    int server =
        tcp_server_listen(
            atoi(argv[1]));



    int fd =
        tcp_server_accept(server);



    benchmark_stats stats;

    stats_reset(&stats);



    run_receiver_benchmark(
        fd,
        atoi(argv[2]),
        atoi(argv[3]),
        &stats);



    printf(
        "RX %.2f Mbps %.2f pps\n",
        stats.throughput_mbps,
        stats.packets_per_second);


    return 0;
}