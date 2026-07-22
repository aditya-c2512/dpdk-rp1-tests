//
// Created by Aditya Choubey on 21/07/2026.
//

#include "stats.h"

#include <string.h>

void stats_reset(benchmark_stats *stats)
{
    memset(stats,0,sizeof(*stats));
}