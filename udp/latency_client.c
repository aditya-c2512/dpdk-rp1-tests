//
// Created by Aditya Choubey on 23/07/2026.
//

#include "benchmark.h"

#include "latency.h"
#include "udp_transport.h"
#include "logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if(argc < 5)
    {
        printf(
            "usage: %s <ip> <port> <samples> <payload_size>\n",
            argv[0]);

        return 1;
    }

    int fd =
        udp_client_connect(
            argv[1],
            atoi(argv[2]));

    if(fd < 0)
    {
        log_error(
            "Failed to connect");

        return 1;
    }

    latency_stats stats =
    {
        0
    };

    uint64_t samples =
        strtoull(argv[3], NULL, 10);

    uint32_t payload =
        atoi(argv[4]);

    if(run_udp_latency_client(
            fd,
            samples,
            payload,
            &stats) < 0)
    {
        log_error(
            "Latency benchmark failed");

        close(fd);
        return 1;
    }

    printf("\nUDP Latency Results\n");
    printf("----------------------------\n");

    printf("Samples : %lu\n", stats.samples);
    printf("Min     : %lu ns\n", stats.min_ns);
    printf("Average : %.2f ns\n", stats.avg_ns);
    printf("Max     : %lu ns\n", stats.max_ns);

    printf("P50     : %lu ns\n", latency_percentile(&stats, 50.0));
    printf("P90     : %lu ns\n", latency_percentile(&stats, 90.0));
    printf("P95     : %lu ns\n", latency_percentile(&stats, 95.0));
    printf("P99     : %lu ns\n", latency_percentile(&stats, 99.0));
    printf("P99.9   : %lu ns\n", latency_percentile(&stats, 99.9));

    latency_csv_append(
        "../results/latency.csv",
        "UDP",
        payload,
        &stats);

    free(stats.samples_ns);

    udp_close(fd);

    return 0;
}