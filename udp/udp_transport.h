//
// Created by Aditya Choubey on 23/07/2026.
//

#ifndef DPDK_RP1_TESTS_UDP_TRANSPORT_H
#define DPDK_RP1_TESTS_UDP_TRANSPORT_H

#include <stdint.h>
#include <sys/types.h>

int udp_client_connect(
    const char *ip,
    uint16_t port);

int udp_server_bind(
    uint16_t port);

ssize_t udp_send(
    int fd,
    const void *buf,
    size_t len);

ssize_t udp_recv(
    int fd,
    void *buf,
    size_t len);

void udp_close(
    int fd);

#endif //DPDK_RP1_TESTS_UDP_TRANSPORT_H