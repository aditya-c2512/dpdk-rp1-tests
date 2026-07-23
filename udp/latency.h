//
// Created by Aditya Choubey on 23/07/2026.
//

#ifndef DPDK_RP1_TESTS_LATENCY_H
#define DPDK_RP1_TESTS_LATENCY_H

#include "packet.h"
#include "stats.h"

#define LATENCY_MAGIC 0xCAFE1234

int run_udp_latency_client(
        int fd,
        uint64_t samples,
        uint32_t payload_size,
        latency_stats *stats);

int run_udp_latency_server(
        int fd);

#endif //DPDK_RP1_TESTS_LATENCY_H