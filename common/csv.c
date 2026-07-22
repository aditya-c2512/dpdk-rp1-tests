//
// Created by Aditya Choubey on 21/07/2026.
//

#include "csv.h"

#include <stdio.h>

int csv_write(
    const char *filename,
    const benchmark_stats *stats)
{
    FILE *fp=fopen(filename,"w");

    if(!fp)
        return -1;

    fprintf(fp,
        "packets_sent,packets_received,bytes_sent,bytes_received,drops,throughput_mbps,pps\n");

    fprintf(fp,
        "%lu,%lu,%lu,%lu,%lu,%f,%f\n",
        stats->packets_sent,
        stats->packets_received,
        stats->bytes_sent,
        stats->bytes_received,
        stats->drops,
        stats->throughput_mbps,
        stats->packets_per_second);

    fclose(fp);

    return 0;
}