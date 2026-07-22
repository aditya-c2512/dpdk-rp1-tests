//
// Created by Aditya Choubey on 21/07/2026.
//
#pragma once

#ifndef DPDK_RP1_TESTS_PACKET_H
#define DPDK_RP1_TESTS_PACKET_H

#include <stdint.h>

typedef struct
{
    uint64_t sequence;
    uint64_t tx_timestamp_ns;
    uint32_t payload_size;
    uint32_t magic;
} benchmark_header;

#endif //DPDK_RP1_TESTS_PACKET_H