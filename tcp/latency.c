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





int run_latency_client(
        int fd,
        uint64_t samples,
        uint32_t packet_size,
        latency_stats *stats)
{
    if(packet_size < sizeof(latency_header))
    {
        log_error(
            "Packet size %u smaller than latency header %lu",
            packet_size,
            sizeof(latency_header));

        return -1;
    }


    uint32_t payload_size =
        packet_size - sizeof(latency_header);



    /*
     * Allocate latency packet
     *
     * Layout:
     *
     * +----------------+
     * | latency_header |
     * +----------------+
     * | payload        |
     * +----------------+
     */
    char *buffer =
        malloc(packet_size);


    if(!buffer)
    {
        log_error(
            "Failed to allocate packet");

        return -1;
    }


    memset(
        buffer,
        0,
        packet_size);



    latency_header *packet =
        (latency_header *)buffer;



    stats->samples_ns =
        malloc(
            sizeof(uint64_t)
            *
            samples);



    if(!stats->samples_ns)
    {
        log_error(
            "Failed to allocate latency histogram");

        free(buffer);

        return -1;
    }



    stats->min_ns = UINT64_MAX;
    stats->max_ns = 0;
    stats->total_ns = 0;
    stats->samples = 0;



    log_info(
        "Starting latency test: samples=%" PRIu64
        " packet_size=%u payload=%u",
        samples,
        packet_size,
        payload_size);



    for(uint64_t i=0;i<samples;i++)
    {

        /*
         * Populate header
         */
        packet->magic =
            LATENCY_MAGIC;


        packet->sequence =
            i;


        packet->timestamp_ns =
            timer_now_ns();


        packet->payload_size =
            payload_size;



        /*
         * Send complete packet
         */
        if(tcp_send_all(
                fd,
                buffer,
                packet_size) < 0)
        {
            log_error(
                "send failed at sample %" PRIu64,
                i);

            free(buffer);

            return -1;
        }



        /*
         * Receive echo
         */
        if(tcp_recv_all(
                fd,
                buffer,
                packet_size) < 0)
        {
            log_error(
                "receive failed");

            free(buffer);

            return -1;
        }



        uint64_t now =
            timer_now_ns();



        /*
         * timestamp_ns was echoed back
         */
        uint64_t latency =
            now -
            packet->timestamp_ns;



        if(latency < stats->min_ns)
            stats->min_ns = latency;



        if(latency > stats->max_ns)
            stats->max_ns = latency;



        stats->samples_ns[
            stats->samples
        ] = latency;



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



    free(buffer);


    return 0;
}