//
// Created by Aditya Choubey on 21/07/2026.
//

#include "timer.h"

#include <time.h>

uint64_t timer_now_ns(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC_RAW,&ts);

    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}