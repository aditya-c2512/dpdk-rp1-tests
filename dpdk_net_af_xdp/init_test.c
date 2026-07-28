//
// Created by Aditya Choubey on 23/07/2026.
//
#include "dpdk_transport.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main()
{
    dpdk_init("eth0");


    char tx[]="hello dpdk";


    dpdk_send(
        tx,
        sizeof(tx));



    char rx[64];


    while(1)
    {
        int ret =
            dpdk_recv(
                rx,
                sizeof(rx));


        if(ret > 0)
        {
            printf(
                "received: %s\n",
                rx);

            break;
        }
    }


    dpdk_close();

    return 0;
}