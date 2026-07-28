//
// Created by Aditya Choubey on 23/07/2026.
//
#include "dpdk_transport.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_mempool.h>
#include <rte_log.h>
#include <rte_mbuf.h>


#define RX_DESC 1024
#define TX_DESC 1024


static uint16_t dpdk_port_id = 0;


static struct rte_mempool *mbuf_pool = NULL;



int dpdk_init(
        const char *iface)
{
    char vdev_arg[128];


    snprintf(
        vdev_arg,
        sizeof(vdev_arg),
        "net_af_xdp0,iface=%s",
        iface);



    char *argv[] =
    {
        "dpdk_benchmark",

        "--vdev",
        vdev_arg,

        "--log-level",
        "7",

        NULL
    };


    int argc = 5;



    int ret =
        rte_eal_init(
            argc,
            argv);


    if(ret < 0)
    {
        fprintf(
            stderr,
            "EAL initialization failed\n");

        return -1;
    }



    uint16_t ports =
        rte_eth_dev_count_avail();



    if(ports == 0)
    {
        fprintf(
            stderr,
            "No DPDK ethernet ports found\n");

        return -1;
    }



    printf(
        "Detected %u DPDK port(s)\n",
        ports);



    dpdk_port_id = 0;



    /*
     * Create mbuf pool
     */
    mbuf_pool =
        rte_pktmbuf_pool_create(
            "benchmark_pool",
            8192,
            256,
            0,
            RTE_MBUF_DEFAULT_BUF_SIZE,
            rte_socket_id());


    if(!mbuf_pool)
    {
        fprintf(
            stderr,
            "Failed creating mbuf pool\n");

        return -1;
    }



    struct rte_eth_conf port_conf =
    {
        0
    };



    ret =
        rte_eth_dev_configure(
            dpdk_port_id,
            1,
            1,
            &port_conf);



    if(ret < 0)
    {
        fprintf(
            stderr,
            "Port configure failed: %d\n",
            ret);

        return -1;
    }



    ret =
        rte_eth_rx_queue_setup(
            dpdk_port_id,
            0,
            RX_DESC,
            rte_eth_dev_socket_id(dpdk_port_id),
            NULL,
            mbuf_pool);



    if(ret < 0)
    {
        fprintf(
            stderr,
            "RX queue setup failed\n");

        return -1;
    }



    ret =
        rte_eth_tx_queue_setup(
            dpdk_port_id,
            0,
            TX_DESC,
            rte_eth_dev_socket_id(dpdk_port_id),
            NULL);



    if(ret < 0)
    {
        fprintf(
            stderr,
            "TX queue setup failed\n");

        return -1;
    }



    ret =
        rte_eth_dev_start(
            dpdk_port_id);

    rte_eth_promiscuous_enable(
            dpdk_port_id);

    if(ret < 0)
    {
        fprintf(
            stderr,
            "Port start failed\n");

        return -1;
    }



    printf(
        "AF_XDP port started successfully\n");


    return 0;
}

ssize_t dpdk_send(
        const void *buf,
        size_t len)
{
    struct rte_mbuf *mbuf;


    mbuf =
        rte_pktmbuf_alloc(
            mbuf_pool);


    if(!mbuf)
    {
        return -1;
    }



    void *data =
        rte_pktmbuf_mtod(
            mbuf,
            void *);



    memcpy(
        data,
        buf,
        len);



    mbuf->data_len =
        len;


    mbuf->pkt_len =
        len;



    struct rte_mbuf *tx_pkts[1];

    tx_pkts[0] = mbuf;



    uint16_t sent =
        rte_eth_tx_burst(
            dpdk_port_id,
            0,
            tx_pkts,
            1);



    if(sent != 1)
    {
        rte_pktmbuf_free(mbuf);

        return -1;
    }



    return len;
}

ssize_t dpdk_recv(
        void *buf,
        size_t len)
{
    struct rte_mbuf *rx_pkts[1];


    uint16_t received =
        rte_eth_rx_burst(
            dpdk_port_id,
            0,
            rx_pkts,
            1);



    if(received == 0)
    {
        return 0;
    }



    struct rte_mbuf *mbuf =
        rx_pkts[0];



    uint16_t pkt_len =
        rte_pktmbuf_pkt_len(
            mbuf);



    if(pkt_len > len)
    {
        rte_pktmbuf_free(mbuf);

        return -1;
    }



    void *data =
        rte_pktmbuf_mtod(
            mbuf,
            void *);



    memcpy(
        buf,
        data,
        pkt_len);



    rte_pktmbuf_free(
        mbuf);



    return pkt_len;
}

void dpdk_close(void)
{
    rte_eth_dev_stop(
        dpdk_port_id);


    rte_eth_dev_close(
        dpdk_port_id);


    printf(
        "DPDK device closed\n");
}