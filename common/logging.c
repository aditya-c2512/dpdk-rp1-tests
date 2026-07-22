//
// Created by Aditya Choubey on 21/07/2026.
//

#include "logging.h"

#include <stdio.h>
#include <stdarg.h>

static void log_generic(
    const char *prefix,
    const char *fmt,
    va_list args)
{
    printf("[%s] ",prefix);

    vprintf(fmt,args);

    printf("\n");
}

void log_info(const char *fmt,...)
{
    va_list args;

    va_start(args,fmt);

    log_generic("INFO",fmt,args);

    va_end(args);
}

void log_error(const char *fmt,...)
{
    va_list args;

    va_start(args,fmt);

    log_generic("ERROR",fmt,args);

    va_end(args);
}