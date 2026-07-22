//
// Created by Aditya Choubey on 21/07/2026.
//

#include "benchmark.h"

#include "tcp_server.h"
#include "throughput.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv)
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


    if(server < 0)
    {
        log_error("Failed to create server");
        return 1;
    }



    int fd =
        tcp_server_accept(server);


    if(fd < 0)
    {
        log_error("Accept failed");
        return 1;
    }



    benchmark_stats stats;

    stats_reset(&stats);



    uint32_t payload_size =
        atoi(argv[2]);

    uint32_t duration =
        atoi(argv[3]);



    run_receiver_benchmark(
        fd,
        payload_size,
        duration,
        &stats);



    printf(
        "RX %.2f Mbps %.2f pps\n",
        stats.throughput_mbps,
        stats.packets_per_second);



    csv_append(
        "../results/rx_throughput.csv",
        "TCP",
        payload_size,
        duration,
        &stats);


    close(fd);
    close(server);


    return 0;
}