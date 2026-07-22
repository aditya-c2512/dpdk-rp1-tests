//
// Created by Aditya Choubey on 22/07/2026.
//

#include "latency.h"

#include "timer.h"
#include "logging.h"

#include "tcp_transport.h"


#include <string.h>
#include <inttypes.h>



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


        stats->total_ns += latency;

        stats->samples++;



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



    return 0;
}