//
// Created by Aditya Choubey on 22/07/2026.
//

#ifndef DPDK_RP1_TESTS_LATENCY_H
#define DPDK_RP1_TESTS_LATENCY_H

#include <stdint.h>

#define LATENCY_MAGIC 0xCAFE1234


typedef struct
{
    uint32_t magic;

    uint64_t sequence;

    uint64_t timestamp_ns;

} latency_packet;


typedef struct
{
    uint64_t samples;

    uint64_t min_ns;
    uint64_t max_ns;

    uint64_t total_ns;

    double avg_ns;

} latency_stats;



int run_latency_client(
        int fd,
        uint64_t samples,
        latency_stats *stats);



int run_latency_server(
        int fd);


#endif //DPDK_RP1_TESTS_LATENCY_H