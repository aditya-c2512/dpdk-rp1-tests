//
// Created by Aditya Choubey on 23/07/2026.
//
#include "throughput.h"

#include "benchmark.h"
#include "packet.h"
#include "timer.h"
#include "logging.h"
#include "udp_transport.h"

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAGIC 0xBEEFCAFE

int run_udp_sender_benchmark(
        int fd,
        uint32_t payload_size,
        uint32_t duration,
        benchmark_stats *stats)
{
    uint32_t size =
        sizeof(benchmark_header) +
        payload_size;

    char *buffer =
        malloc(size);

    if(!buffer)
        return -1;

    memset(buffer, 0, size);

    benchmark_header *hdr =
        (benchmark_header *)buffer;

    hdr->magic = MAGIC;
    hdr->payload_size = payload_size;

    uint64_t start =
        timer_now_ns();

    uint64_t end =
        start +
        duration * 1000000000ULL;

    uint64_t seq = 0;
    uint64_t last_log = start;

    log_info(
        "Starting UDP sender: payload=%u total=%u duration=%u",
        payload_size,
        size,
        duration);

    while(timer_now_ns() < end)
    {
        hdr->sequence = seq++;
        hdr->tx_timestamp_ns =
            timer_now_ns();

        ssize_t ret =
            udp_send(
                fd,
                buffer,
                size);

        if(ret != (ssize_t)size)
        {
            stats->errors++;
            break;
        }

        stats->packets_sent++;
        stats->bytes_sent += size;
        stats->send_calls++;

        uint64_t now =
            timer_now_ns();

        if(now - last_log >= 1000000000ULL)
        {
            double elapsed =
                (now - start) / 1e9;

            log_info(
                "TX packets=%" PRIu64
                " bytes=%" PRIu64
                " rate=%.2f Mbps",
                stats->packets_sent,
                stats->bytes_sent,
                (stats->bytes_sent * 8.0) /
                elapsed /
                1000000.0);

            last_log = now;
        }
    }

    stats->runtime_ns =
        timer_now_ns() - start;

    stats->throughput_mbps =
        (stats->bytes_sent * 8.0) /
        (stats->runtime_ns / 1e9) /
        1000000.0;

    stats->packets_per_second =
        stats->packets_sent /
        (stats->runtime_ns / 1e9);

    free(buffer);

    return 0;
}

int run_udp_receiver_benchmark(
        int fd,
        uint32_t payload_size,
        uint32_t duration,
        benchmark_stats *stats)
{
    uint32_t size =
        sizeof(benchmark_header) +
        payload_size;

    char *buffer =
        malloc(size);

    if(!buffer)
        return -1;

    benchmark_header *hdr =
        (benchmark_header *)buffer;

    uint64_t expected_seq = 0;

    uint64_t start =
        timer_now_ns();

    uint64_t end =
        start +
        duration * 1000000000ULL;

    uint64_t last_log = start;

    log_info(
        "Starting UDP receiver: packet=%u",
        size);

    while(timer_now_ns() < end)
    {
        ssize_t ret =
            udp_recv(
                fd,
                buffer,
                size);

        if(ret < 0)
        {
            stats->errors++;
            continue;
        }

        if((uint32_t)ret != size)
        {
            stats->errors++;
            continue;
        }

        if(hdr->magic != MAGIC)
        {
            stats->errors++;
            continue;
        }

        /*
         * Sequence tracking
         */
        if(hdr->sequence != expected_seq)
        {
            /*
             * Placeholder:
             * packet loss / out-of-order accounting
             * can be added here later.
             */
            expected_seq =
                hdr->sequence + 1;
        }
        else
        {
            expected_seq++;
        }

        stats->packets_received++;
        stats->bytes_received += ret;
        stats->recv_calls++;

        uint64_t now =
            timer_now_ns();

        if(now - last_log >= 1000000000ULL)
        {
            double elapsed =
                (now - start) / 1e9;

            log_info(
                "RX packets=%" PRIu64
                " bytes=%" PRIu64
                " rate=%.2f Mbps",
                stats->packets_received,
                stats->bytes_received,
                (stats->bytes_received * 8.0) /
                elapsed /
                1000000.0);

            last_log = now;
        }
    }

    stats->runtime_ns =
        timer_now_ns() - start;

    stats->throughput_mbps =
        (stats->bytes_received * 8.0) /
        (stats->runtime_ns / 1e9) /
        1000000.0;

    stats->packets_per_second =
        stats->packets_received /
        (stats->runtime_ns / 1e9);

    free(buffer);

    return 0;
}