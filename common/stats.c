//
// Created by Aditya Choubey on 21/07/2026.
//

#include "stats.h"

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