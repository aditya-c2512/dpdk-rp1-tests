//
// Created by Aditya Choubey on 21/07/2026.
//
#pragma once

#ifndef DPDK_RP1_TESTS_TCP_CLIENT_H
#define DPDK_RP1_TESTS_TCP_CLIENT_H

#include <stdint.h>

int tcp_server_listen(
        uint16_t port);

int tcp_server_accept(
        int server_fd);

#endif //DPDK_RP1_TESTS_TCP_CLIENT_H