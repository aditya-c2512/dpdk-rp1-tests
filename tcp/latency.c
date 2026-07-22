//
// Created by Aditya Choubey on 22/07/2026.
//

#include "latency.h"
#include "packet.h"
#include "timer.h"
#include "logging.h"

#include "tcp_transport.h"

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

static int compare_uint64(
        const void *a,
        const void *b)
{
    uint64_t x = *(uint64_t *)a;
    uint64_t y = *(uint64_t *)b;


    if(x < y)
        return -1;

    if(x > y)
        return 1;

    return 0;
}



void latency_sort(
        latency_stats *stats)
{
    qsort(
        stats->samples_ns,
        stats->samples,
        sizeof(uint64_t),
        compare_uint64);
}



uint64_t latency_percentile(
        latency_stats *stats,
        double percentile)
{
    if(stats->samples == 0)
        return 0;


    uint64_t index =
        (uint64_t)
        ((percentile / 100.0)
        *
        (stats->samples - 1));


    return stats->samples_ns[index];
}

int run_latency_client(
        int fd,
        uint64_t samples,
        latency_stats *stats)
{
    latency_packet packet;


    memset(
        &packet,
        0,
        sizeof(packet));


    stats->samples_ns =
    malloc(
        sizeof(uint64_t)
        *
        samples);


    if(!stats->samples_ns)
    {
        log_error(
            "Failed to allocate latency histogram");

        return -1;
    }


    stats->min_ns = UINT64_MAX;



    log_info(
        "Starting latency test: samples=%" PRIu64,
        samples);



    for(uint64_t i=0;i<samples;i++)
    {
        packet.magic =
            LATENCY_MAGIC;


        packet.sequence =
            i;


        packet.timestamp_ns =
            timer_now_ns();



        if(tcp_send_all(
                fd,
                &packet,
                sizeof(packet)) < 0)
        {
            log_error(
                "send failed at sample %" PRIu64,
                i);

            return -1;
        }



        if(tcp_recv_all(
                fd,
                &packet,
                sizeof(packet)) < 0)
        {
            log_error(
                "receive failed");

            return -1;
        }



        uint64_t now =
            timer_now_ns();



        uint64_t latency =
            now - packet.timestamp_ns;



        if(latency < stats->min_ns)
            stats->min_ns = latency;


        if(latency > stats->max_ns)
            stats->max_ns = latency;


        stats->samples_ns[stats->samples] = latency;


        stats->samples++;


        stats->total_ns += latency;



        if(i % 10000 == 0)
        {
            log_info(
                "sample=%" PRIu64
                " latency=%" PRIu64 " ns",
                i,
                latency);
        }
    }



    stats->avg_ns =
    (double)stats->total_ns /
    stats->samples;


    latency_sort(stats);

    return 0;
}