//
// Created by Aditya Choubey on 21/07/2026.
//
#pragma once

#ifndef DPDK_RP1_TESTS_TCP_CLIENT_H
#define DPDK_RP1_TESTS_TCP_CLIENT_H

#include <stdint.h>


int tcp_client_connect(
        const char *address,
        uint16_t port);


int tcp_send_all(
        int fd,
        const void *buffer,
        uint32_t length);


int tcp_recv_all(
        int fd,
        void *buffer,
        uint32_t length);

#endif //DPDK_RP1_TESTS_TCP_CLIENT_H