//
// Created by Aditya Choubey on 23/07/2026.
//

#ifndef DPDK_RP1_TESTS_DPDK_TRANSPORT_H
#define DPDK_RP1_TESTS_DPDK_TRANSPORT_H

int dpdk_init(
        const char *iface);

ssize_t dpdk_send(
        const void *buf,
        size_t len);


ssize_t dpdk_recv(
        void *buf,
        size_t len);

void dpdk_close(void);

#endif //DPDK_RP1_TESTS_DPDK_TRANSPORT_H