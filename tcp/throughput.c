//
// Created by Aditya Choubey on 21/07/2026.
//

#include "throughput.h"

#include "packet.h"
#include "timer.h"
#include "logging.h"
#include "tcp_client.h"

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

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


    char *buffer = malloc(size);

    if (!buffer)
    {
        log_error("Failed to allocate TX buffer");
        return -1;
    }


    benchmark_header *hdr =
        (benchmark_header *)buffer;


    memset(buffer, 0, size);


    hdr->magic = MAGIC;
    hdr->payload_size = payload_size;


    log_info(
        "Starting TCP sender: payload=%u bytes total_packet=%u bytes duration=%u seconds",
        payload_size,
        size,
        duration);


    uint64_t start =
        timer_now_ns();

    uint64_t end =
        start +
        duration * 1000000000ULL;


    uint64_t seq = 0;

    uint64_t last_report =
        start;


    while(timer_now_ns() < end)
    {
        hdr->sequence = seq++;

        hdr->tx_timestamp_ns =
            timer_now_ns();



        int rc =
            tcp_send_all(
                fd,
                buffer,
                size);


        if(rc < 0)
        {
            log_error(
                "send failed at packet %" PRIu64,
                seq);

            stats->errors++;
            break;
        }



        stats->packets_sent++;

        stats->bytes_sent += size;

        stats->send_calls++;



        /*
         * Print first packet
         */
        if(seq == 1)
        {
            log_info(
                "First packet sent: seq=%" PRIu64 " size=%u",
                hdr->sequence,
                size);
        }



        /*
         * Periodic statistics
         */
        uint64_t now =
            timer_now_ns();


        if(now - last_report >= 1000000000ULL)
        {
            double elapsed =
                (now-start)/1e9;


            log_info(
                "TX packets=%" PRIu64
                " bytes=%" PRIu64
                " rate=%.2f Mbps",
                stats->packets_sent,
                stats->bytes_sent,
                (stats->bytes_sent*8.0)
                    / elapsed
                    / 1000000.0);


            last_report = now;
        }
    }



    stats->runtime_ns =
        timer_now_ns()-start;



    stats->throughput_mbps =
        (stats->bytes_sent*8.0)
        /
        (stats->runtime_ns/1e9)
        /
        1000000.0;


    stats->packets_per_second =
        stats->packets_sent
        /
        (stats->runtime_ns/1e9);



    log_info(
        "Sender complete: packets=%" PRIu64
        " bytes=%" PRIu64
        " throughput=%.2f Mbps",
        stats->packets_sent,
        stats->bytes_sent,
        stats->throughput_mbps);



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


    if(!buffer)
    {
        log_error("Failed to allocate RX buffer");
        return -1;
    }


    log_info(
        "Starting TCP receiver: expecting packet size=%u bytes",
        size);



    uint64_t start =
        timer_now_ns();


    uint64_t last_report =
        start;



    while(timer_now_ns()
          <
          start + duration*1000000000ULL)
    {

        if(tcp_recv_all(
                fd,
                buffer,
                size)<0)
        {
            log_error(
                "recv failed");

            break;
        }



        benchmark_header *hdr =
            (benchmark_header *)buffer;


        if(stats->packets_received == 0)
        {
            log_info(
                "First packet received: magic=0x%x seq=%" PRIu64,
                hdr->magic,
                hdr->sequence);
        }


        if(hdr->magic != MAGIC)
        {
            log_error(
                "Invalid packet magic: 0x%x",
                hdr->magic);
        }



        stats->packets_received++;

        stats->bytes_received += size;

        stats->recv_calls++;



        uint64_t now =
            timer_now_ns();


        if(now-last_report >= 1000000000ULL)
        {
            double elapsed =
                (now-start)/1e9;


            log_info(
                "RX packets=%" PRIu64
                " bytes=%" PRIu64
                " rate=%.2f Mbps",
                stats->packets_received,
                stats->bytes_received,
                (stats->bytes_received*8.0)
                    /
                    elapsed
                    /
                    1000000.0);


            last_report = now;
        }
    }



    stats->runtime_ns =
        timer_now_ns()-start;


    stats->throughput_mbps =
        (stats->bytes_received*8.0)
        /
        (stats->runtime_ns/1e9)
        /
        1000000.0;


    stats->packets_per_second =
        stats->packets_received
        /
        (stats->runtime_ns/1e9);



    log_info(
        "Receiver complete: packets=%" PRIu64
        " bytes=%" PRIu64
        " throughput=%.2f Mbps",
        stats->packets_received,
        stats->bytes_received,
        stats->throughput_mbps);



    free(buffer);

    return 0;
}