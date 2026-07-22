//
// Created by Aditya Choubey on 21/07/2026.
//
#pragma once

#ifndef DPDK_RP1_TESTS_CSV_H
#define DPDK_RP1_TESTS_CSV_H

#include "stats.h"

int throughput_csv_write(const char *filename, const benchmark_stats *stats);

int throughput_csv_append(
    const char *filename,
    const char *test_type,
    uint32_t packet_size,
    uint32_t duration_seconds,
    const benchmark_stats *stats);

int latency_csv_append(
        const char *filename,
        const char *test_type,
        latency_stats *stats);

#endif //DPDK_RP1_TESTS_CSV_H