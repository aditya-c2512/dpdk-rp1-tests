//
// Created by Aditya Choubey on 23/07/2026.
//

#ifndef DPDK_RP1_TESTS_THROUGHPUT_H
#define DPDK_RP1_TESTS_THROUGHPUT_H

#include <stdint.h>

#include "stats.h"

int run_udp_sender_benchmark(
        int fd,
        uint32_t payload_size,
        uint32_t duration,
        benchmark_stats *stats);

int run_udp_receiver_benchmark(
        int fd,
        uint32_t payload_size,
        uint32_t duration,
        benchmark_stats *stats);

#endif //DPDK_RP1_TESTS_THROUGHPUT_H