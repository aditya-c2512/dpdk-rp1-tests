//
// Created by Aditya Choubey on 23/07/2026.
//
#include "latency.h"

#include <inttypes.h>
#include <stdlib.h>

#include "logging.h"
#include "timer.h"
#include "udp_transport.h"


int run_udp_latency_client(
        int fd,
        uint64_t samples,
        uint32_t payload_size,
        latency_stats *stats)
{
    uint32_t packet_size =
        sizeof(latency_header) +
        payload_size;

    char *buffer = calloc(1, packet_size);

    latency_header *hdr =
        (latency_header *)buffer;

    stats->samples_ns =
        malloc(samples * sizeof(uint64_t));

    stats->min_ns = UINT64_MAX;

    log_info(
        "Starting UDP latency test (%" PRIu64 " samples, %u-byte payload)",
        samples,
        payload_size);

    for(uint64_t i=0;i<samples;i++)
    {
        hdr->magic = LATENCY_MAGIC;
        hdr->sequence = i;
        hdr->payload_size = payload_size;
        hdr->timestamp_ns = timer_now_ns();

        if(udp_send(fd, buffer, packet_size) != packet_size)
            return -1;

        if(udp_recv(fd, buffer, packet_size) != packet_size)
            return -1;

        uint64_t latency =
            timer_now_ns() -
            hdr->timestamp_ns;

        stats->samples_ns[stats->samples++] =
            latency;

        stats->total_ns += latency;

        if(latency < stats->min_ns)
            stats->min_ns = latency;

        if(latency > stats->max_ns)
            stats->max_ns = latency;

        if(i % 10000 == 0)
            log_info("sample=%" PRIu64, i);
    }

    stats->avg_ns =
        (double)stats->total_ns /
        stats->samples;

    latency_sort(stats);

    free(buffer);

    return 0;
}

int run_udp_latency_server(
        int fd)
{
    char buffer[2048];

    while(1)
    {
        ssize_t len =
            udp_recv(fd,
                     buffer,
                     sizeof(buffer));

        if(len <= 0)
            continue;

        latency_header *hdr =
            (latency_header *)buffer;

        if(hdr->magic != LATENCY_MAGIC)
            continue;

        udp_send(fd,
                 buffer,
                 len);
    }

    return 0;
}