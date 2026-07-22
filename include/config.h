//
// Created by Aditya Choubey on 21/07/2026.
//
#pragma once

#ifndef DPDK_RP1_TESTS_CONFIG_H
#define DPDK_RP1_TESTS_CONFIG_H

#include <stdint.h>

typedef struct
{
    char server_ip[64];

    uint16_t port;

    uint32_t packet_size;

    uint32_t duration;

    uint32_t rate_mbps;

} benchmark_config;

#endif //DPDK_RP1_TESTS_CONFIG_H