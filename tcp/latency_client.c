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


int main(int argc,char **argv)
{
    if(argc < 4)
    {
        printf(
        "usage: %s <ip> <port> <samples>\n",
        argv[0]);

        return 1;
    }


    int fd =
        tcp_client_connect(
            argv[1],
            atoi(argv[2]));



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
        strtoull(argv[3],NULL,10),
        &stats);



    printf("\nLatency Results\n");
    printf("-----------------------\n");

    printf(
        "Samples : %lu\n",
        stats.samples);

    printf(
        "Min     : %lu ns\n",
        stats.min_ns);

    printf(
        "Average : %.2f ns\n",
        stats.avg_ns);

    printf(
        "Max     : %lu ns\n",
        stats.max_ns);



    close(fd);

    return 0;
}