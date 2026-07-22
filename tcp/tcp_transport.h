//
// Created by Aditya Choubey on 22/07/2026.
//

#ifndef DPDK_RP1_TESTS_TCP_TRANSPORT_H
#define DPDK_RP1_TESTS_TCP_TRANSPORT_H

#include <stdint.h>


int tcp_send_all(
        int fd,
        const void *buffer,
        uint32_t length);


int tcp_recv_all(
        int fd,
        void *buffer,
        uint32_t length);

#endif //DPDK_RP1_TESTS_TCP_TRANSPORT_H