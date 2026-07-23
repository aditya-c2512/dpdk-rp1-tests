//
// Created by Aditya Choubey on 21/07/2026.
//

#include "stats.h"

#include <stdlib.h>
#include <string.h>

void stats_reset(benchmark_stats *stats)
{
    memset(stats,0,sizeof(*stats));
}

uint64_t latency_percentile(
        latency_stats *stats,
        double percentile)
{
    if(stats->samples == 0)
        return 0;


    uint64_t index =
        (uint64_t)
        ((percentile / 100.0)
        *
        (stats->samples - 1));


    return stats->samples_ns[index];
}

static int compare_uint64(
        const void *a,
        const void *b)
{
    uint64_t x = *(uint64_t *)a;
    uint64_t y = *(uint64_t *)b;


    if(x < y)
        return -1;

    if(x > y)
        return 1;

    return 0;
}

void latency_sort(
        latency_stats *stats)
{
    qsort(
        stats->samples_ns,
        stats->samples,
        sizeof(uint64_t),
        compare_uint64);
}