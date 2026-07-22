//
// Created by Aditya Choubey on 21/07/2026.
//

#include "throughput.h"

#include "packet.h"
#include "timer.h"

#include "tcp_client.h"

#include <stdlib.h>
#include <string.h>

#define MAGIC 0xBEEFCAFE

int run_sender_benchmark(
        int fd,
        uint32_t payload_size,
        uint32_t duration,
        benchmark_stats *stats)
{

    uint32_t size =
        sizeof(benchmark_header)
        + payload_size;


    char *buffer =
        malloc(size);



    benchmark_header *hdr =
        (benchmark_header *)buffer;


    memset(
        buffer,
        0,
        size);



    hdr->magic = MAGIC;

    hdr->payload_size =
        payload_size;



    uint64_t start =
        timer_now_ns();



    uint64_t end =
        start +
        duration * 1000000000ULL;



    uint64_t seq=0;



    while(timer_now_ns()<end)
    {
        hdr->sequence=seq++;

        hdr->tx_timestamp_ns =
            timer_now_ns();



        if(tcp_send_all(
                fd,
                buffer,
                size)<0)
        {
            stats->errors++;
            break;
        }


        stats->packets_sent++;

        stats->bytes_sent += size;

        stats->send_calls++;
    }



    stats->runtime_ns =
        timer_now_ns()-start;


    stats->throughput_mbps =
        (stats->bytes_sent*8.0) /
        (stats->runtime_ns/1e9) /
        1000000.0;


    stats->packets_per_second =
        stats->packets_sent /
        (stats->runtime_ns/1e9);



    free(buffer);


    return 0;
}





int run_receiver_benchmark(
        int fd,
        uint32_t payload_size,
        uint32_t duration,
        benchmark_stats *stats)
{

    uint32_t size =
        sizeof(benchmark_header)
        + payload_size;



    char *buffer =
        malloc(size);



    uint64_t start =
        timer_now_ns();



    while(
        timer_now_ns()
        <
        start + duration*1000000000ULL)
    {

        if(tcp_recv_all(
                fd,
                buffer,
                size)<0)
        {
            break;
        }

        stats->packets_received++;
        stats->bytes_received += size;
        stats->recv_calls++;
    }



    stats->runtime_ns =
        timer_now_ns()-start;



    stats->throughput_mbps =
        (stats->bytes_received*8.0) /
        (stats->runtime_ns/1e9) /
        1000000.0;



    stats->packets_per_second =
        stats->packets_received /
        (stats->runtime_ns/1e9);



    free(buffer);


    return 0;
}