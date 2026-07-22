//
// Created by Aditya Choubey on 21/07/2026.
//
#pragma once

#ifndef DPDK_RP1_TESTS_THROUGHPUT_H
#define DPDK_RP1_TESTS_THROUGHPUT_H

#include "stats.h"

int run_sender_benchmark(
        int fd,
        uint32_t payload_size,
        uint32_t duration,
        benchmark_stats *stats);



int run_receiver_benchmark(
        int fd,
        uint32_t payload_size,
        uint32_t duration,
        benchmark_stats *stats);

#endif //DPDK_RP1_TESTS_THROUGHPUT_H