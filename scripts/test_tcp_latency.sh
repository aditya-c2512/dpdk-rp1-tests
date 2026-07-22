#!/bin/bash

set -e


WAIT_TIME=5

PACKET_SIZES=(64 128 256 512 1024 1500)


usage()
{
    echo "Usage:"
    echo
    echo "TX:"
    echo "  $0 <runs> tx <ip> <port> <samples> <packet_size>"
    echo "  $0 <runs> tx <ip> <port> <samples> sweep"
    echo
    echo "RX:"
    echo "  $0 <runs> rx <port>"
    exit 1
}



if [ $# -lt 2 ]; then
    usage
fi


RUNS=$1
MODE=$2


SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"


CLIENT_BIN="${SCRIPT_DIR}/tcp_latency_client"
SERVER_BIN="${SCRIPT_DIR}/tcp_latency_server"



run_latency_tx()
{
    IP=$1
    PORT=$2
    SAMPLES=$3
    PACKET_SIZE=$4


    echo "================================"
    echo "TCP Latency Client"
    echo "================================"
    echo "Packet size: $PACKET_SIZE bytes"
    echo "Samples/run: $SAMPLES"
    echo



    for ((i=1;i<=RUNS;i++))
    do

        echo "================================"
        echo "Latency TX Run $i/$RUNS"
        echo "Packet size: $PACKET_SIZE"
        echo "================================"



        $CLIENT_BIN \
            $IP \
            $PORT \
            $SAMPLES \
            $PACKET_SIZE



        echo
        echo "Completed latency TX run $i/$RUNS"



        if [ $i -lt $RUNS ]; then
            echo "Waiting ${WAIT_TIME}s..."
            sleep $WAIT_TIME
        fi

    done
}



if [ "$MODE" == "tx" ]; then


    if [ $# -ne 6 ]; then
        usage
    fi


    IP=$3
    PORT=$4
    SAMPLES=$5
    SIZE=$6



    echo "================================"
    echo "TCP Latency Benchmark"
    echo "================================"
    echo "Runs: $RUNS"
    echo "Destination: $IP:$PORT"
    echo "Samples/run: $SAMPLES"
    echo



    if [ "$SIZE" == "sweep" ]; then


        echo "Starting packet size sweep"


        for PACKET_SIZE in "${PACKET_SIZES[@]}"
        do

            echo
            echo "################################"
            echo "Testing packet size ${PACKET_SIZE}"
            echo "################################"


            run_latency_tx \
                $IP \
                $PORT \
                $SAMPLES \
                $PACKET_SIZE


            echo
            echo "Completed packet size ${PACKET_SIZE}"


            sleep $WAIT_TIME

        done


    else


        run_latency_tx \
            $IP \
            $PORT \
            $SAMPLES \
            $SIZE

    fi



elif [ "$MODE" == "rx" ]; then


    if [ $# -ne 3 ]; then
        usage
    fi


    PORT=$3


    echo "================================"
    echo "TCP Latency Server"
    echo "================================"
    echo "Runs: $RUNS"
    echo "Port: $PORT"
    echo



    for ((i=1;i<=RUNS;i++))
    do

        echo "================================"
        echo "Latency RX Run $i/$RUNS"
        echo "================================"



        $SERVER_BIN \
            $PORT



        echo
        echo "Completed latency RX run $i/$RUNS"



        if [ $i -lt $RUNS ]; then
            echo "Waiting ${WAIT_TIME}s..."
            sleep $WAIT_TIME
        fi

    done


else

    usage

fi


echo
echo "Latency benchmark complete."