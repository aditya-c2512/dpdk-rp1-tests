//
// Created by Aditya Choubey on 21/07/2026.
//
#pragma once

#ifndef DPDK_RP1_TESTS_STATS_H
#define DPDK_RP1_TESTS_STATS_H

#include <stdint.h>

typedef struct
{
    uint64_t packets_sent;
    uint64_t packets_received;

    uint64_t bytes_sent;
    uint64_t bytes_received;

    uint64_t send_calls;
    uint64_t recv_calls;

    uint64_t partial_sends;
    uint64_t partial_recvs;

    uint64_t errors;

    uint64_t runtime_ns;

    double throughput_mbps;
    double packets_per_second;

} benchmark_stats;

typedef struct
{
    uint64_t samples;

    uint64_t *samples_ns;

    uint64_t min_ns;
    uint64_t max_ns;

    uint64_t total_ns;
    double avg_ns;

} latency_stats;

void stats_reset(benchmark_stats *stats);

#endif //DPDK_RP1_TESTS_STATS_H