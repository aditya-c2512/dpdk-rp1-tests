//
// Created by Aditya Choubey on 22/07/2026.
//

#ifndef DPDK_RP1_TESTS_LATENCY_H
#define DPDK_RP1_TESTS_LATENCY_H

#include "stats.h"
#include <stdint.h>

#define LATENCY_MAGIC 0xCAFE1234


#define MAX_LATENCY_SAMPLES 10000000



int run_latency_client(
        int fd,
        uint64_t samples,
        latency_stats *stats);



int run_latency_server(
        int fd);


#endif //DPDK_RP1_TESTS_LATENCY_H