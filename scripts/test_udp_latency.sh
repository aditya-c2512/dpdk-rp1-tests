#!/bin/bash

set -e

WAIT_TIME=5

PACKET_SIZES=(64 128 256 512 1024 1500)


usage()
{
    echo "Usage:"
    echo
    echo "TX:"
    echo "  $0 <runs> tx <ip> <port> <samples> <packet_size|sweep>"
    echo
    echo "RX:"
    echo "  $0 <runs> rx <port>"
    echo
    echo "Examples:"
    echo "  $0 10 tx 192.168.100.1 5000 100000 512"
    echo "  $0 10 tx 192.168.100.1 5000 100000 sweep"
    echo "  $0 10 rx 5000"
    exit 1
}


if [ $# -lt 2 ]; then
    usage
fi


RUNS=$1
MODE=$2


SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

CLIENT_BIN="${SCRIPT_DIR}/../build/udp_latency_client"
SERVER_BIN="${SCRIPT_DIR}/../build/udp_latency_server"



run_latency_client()
{
    SIZE=$1

    echo "================================"
    echo "UDP Latency TX"
    echo "Packet size: ${SIZE} bytes"
    echo "================================"


    for ((i=1;i<=RUNS;i++))
    do

        echo
        echo "Latency TX Run $i/$RUNS"
        echo "Packet size: ${SIZE}"


        $CLIENT_BIN \
            $IP \
            $PORT \
            $SAMPLES \
            $SIZE


        echo
        echo "Completed run $i/$RUNS"


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
    PACKET_SIZE=$6


    echo "================================"
    echo "UDP Latency Client"
    echo "================================"
    echo "Runs: $RUNS"
    echo "Destination: $IP:$PORT"
    echo "Samples/run: $SAMPLES"
    echo


    if [ "$PACKET_SIZE" == "sweep" ]; then

        echo "Running packet size sweep"

        for SIZE in "${PACKET_SIZES[@]}"
        do
            run_latency_client $SIZE
        done

    else

        run_latency_client $PACKET_SIZE

    fi



elif [ "$MODE" == "rx" ]; then


    if [ $# -ne 3 ]; then
        usage
    fi


    PORT=$3


    echo "================================"
    echo "UDP Latency Server"
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
echo "UDP latency benchmark complete."