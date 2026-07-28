//
// Created by Aditya Choubey on 23/07/2026.
//

#ifndef DPDK_RP1_TESTS_UDP_TRANSPORT_H
#define DPDK_RP1_TESTS_UDP_TRANSPORT_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/_types/_socklen_t.h>

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

ssize_t udp_recv_from(
        int fd,
        void *buf,
        size_t len,
        struct sockaddr_in *peer,
        socklen_t *peer_len);

ssize_t udp_send_to(
        int fd,
        const void *buf,
        size_t len,
        const struct sockaddr_in *peer,
        socklen_t peer_len);


void udp_close(
    int fd);

#endif //DPDK_RP1_TESTS_UDP_TRANSPORT_H