//
// Created by Aditya Choubey on 21/07/2026.
//

#define _GNU_SOURCE

#include "affinity.h"

#include <sched.h>

int pin_to_cpu(int cpu)
{
    cpu_set_t set;

    CPU_ZERO(&set);

    CPU_SET(cpu,&set);

    return sched_setaffinity(
        0,
        sizeof(set),
        &set);
}