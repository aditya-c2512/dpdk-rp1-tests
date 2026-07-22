//
// Created by Aditya Choubey on 21/07/2026.
//

#include "benchmark.h"

#include "latency.h"

#include "tcp_client.h"
#include "logging.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>


int main(int argc,char **argv)
{
    if(argc < 5)
    {
        printf(
        "usage: %s <ip> <port> <samples> <packet_size>\n",
        argv[0]);

        return 1;
    }



    const char *ip =
        argv[1];


    uint16_t port =
        atoi(argv[2]);


    uint64_t samples =
        strtoull(
            argv[3],
            NULL,
            10);


    uint32_t packet_size =
        atoi(argv[4]);



    int fd =
        tcp_client_connect(
            ip,
            port);



    if(fd < 0)
    {
        log_error(
            "Connection failed");

        return 1;
    }



    latency_stats stats =
    {
        0
    };



    run_latency_client(
        fd,
        samples,
        packet_size,
        &stats);



    printf("\nLatency Results\n");
    printf("-----------------------\n");


    printf(
        "Packet size : %u bytes\n",
        packet_size);


    printf(
        "Samples     : %" PRIu64 "\n",
        stats.samples);



    printf(
        "Min         : %" PRIu64 " ns\n",
        stats.min_ns);



    printf(
        "Average     : %.2f ns\n",
        stats.avg_ns);



    printf(
        "Max         : %" PRIu64 " ns\n",
        stats.max_ns);



    printf(
        "p50         : %" PRIu64 " ns\n",
        latency_percentile(
            &stats,
            50));


    printf(
        "p99         : %" PRIu64 " ns\n",
        latency_percentile(
            &stats,
            99));


    latency_csv_append(
        "../results/latency.csv",
        "TCP",
        packet_size,
        &stats);



    free(stats.samples_ns);


    close(fd);


    return 0;
}