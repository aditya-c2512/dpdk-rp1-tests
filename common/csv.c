//
// Created by Aditya Choubey on 21/07/2026.
//

#include "csv.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <time.h>

int csv_write(const char *filename, const benchmark_stats *stats)
{
    FILE *fp = fopen(filename, "w");
    if (!fp)
        return -1;

    /* CSV header */
    fprintf(fp,
        "packets_sent,"
        "packets_received,"
        "bytes_sent,"
        "bytes_received,"
        "send_calls,"
        "recv_calls,"
        "partial_sends,"
        "partial_recvs,"
        "errors,"
        "runtime_ns,"
        "throughput_mbps,"
        "packets_per_second\n");

    /* CSV values */
    fprintf(fp,
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%.6f,"
        "%.6f\n",
        stats->packets_sent,
        stats->packets_received,
        stats->bytes_sent,
        stats->bytes_received,
        stats->send_calls,
        stats->recv_calls,
        stats->partial_sends,
        stats->partial_recvs,
        stats->errors,
        stats->runtime_ns,
        stats->throughput_mbps,
        stats->packets_per_second);

    fclose(fp);
    return 0;
}

int csv_append(
    const char *filename,
    const char *test_type,
    uint32_t packet_size,
    uint32_t duration_seconds,
    const benchmark_stats *stats)
{
    bool new_file = false;

    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        new_file = true;
    }
    else
    {
        fclose(fp);
    }

    fp = fopen(filename, "a");
    if (!fp)
        return -1;

    if (new_file)
    {
        fprintf(fp,
            "timestamp,"
            "test_type,"
            "packet_size,"
            "duration_seconds,"
            "packets_sent,"
            "packets_received,"
            "bytes_sent,"
            "bytes_received,"
            "send_calls,"
            "recv_calls,"
            "partial_sends,"
            "partial_recvs,"
            "errors,"
            "runtime_ns,"
            "throughput_mbps,"
            "packets_per_second\n");
    }

    char timestamp[32];
    time_t now = time(NULL);

    struct tm tm_now;
    localtime_r(&now, &tm_now);

    strftime(
        timestamp,
        sizeof(timestamp),
        "%Y-%m-%d %H:%M:%S",
        &tm_now);

    fprintf(fp,
        "%s,"
        "%s,"
        "%u,"
        "%u,"
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%" PRIu64 ","
        "%.6f,"
        "%.6f\n",

        timestamp,
        test_type,
        packet_size,
        duration_seconds,

        stats->packets_sent,
        stats->packets_received,

        stats->bytes_sent,
        stats->bytes_received,

        stats->send_calls,
        stats->recv_calls,

        stats->partial_sends,
        stats->partial_recvs,

        stats->errors,

        stats->runtime_ns,

        stats->throughput_mbps,
        stats->packets_per_second);

    fclose(fp);

    return 0;
}
