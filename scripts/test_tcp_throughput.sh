#!/bin/bash

set -e


WAIT_TIME=5


usage()
{
    echo "Usage:"
    echo "TX:"
    echo "  $0 <runs> tx <ip> <port> <packet_size> <duration>"
    echo
    echo "RX:"
    echo "  $0 <runs> rx <port> <packet_size> <duration>"
    exit 1
}



if [ $# -lt 2 ]; then
    usage
fi


RUNS=$1
MODE=$2


SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"


TX_BIN="${SCRIPT_DIR}/tcp_sender"
RX_BIN="${SCRIPT_DIR}/tcp_receiver"



if [ "$MODE" == "tx" ]; then

    if [ $# -ne 6 ]; then
        usage
    fi


    IP=$3
    PORT=$4
    SIZE=$5
    DURATION=$6


    echo "Starting TCP TX benchmark"
    echo "Runs: $RUNS"
    echo "Destination: $IP:$PORT"
    echo "Packet size: $SIZE bytes"
    echo "Duration: $DURATION seconds"
    echo


    for ((i=1;i<=RUNS;i++))
    do
        echo "================================"
        echo "TX Run $i/$RUNS"
        echo "================================"


        $TX_BIN \
            $IP \
            $PORT \
            $SIZE \
            $DURATION


        echo
        echo "Completed TX run $i/$RUNS"


        if [ $i -lt $RUNS ]; then
            echo "Waiting ${WAIT_TIME}s..."
            sleep $WAIT_TIME
        fi
    done



elif [ "$MODE" == "rx" ]; then


    if [ $# -ne 5 ]; then
        usage
    fi


    PORT=$3
    SIZE=$4
    DURATION=$5


    echo "Starting TCP RX benchmark"
    echo "Runs: $RUNS"
    echo "Listening port: $PORT"
    echo "Packet size: $SIZE bytes"
    echo "Duration: $DURATION seconds"
    echo



    for ((i=1;i<=RUNS;i++))
    do
        echo "================================"
        echo "RX Run $i/$RUNS"
        echo "================================"


        $RX_BIN \
            $PORT \
            $SIZE \
            $DURATION


        echo
        echo "Completed RX run $i/$RUNS"


        if [ $i -lt $RUNS ]; then
            echo "Waiting ${WAIT_TIME}s..."
            sleep $WAIT_TIME
        fi
    done



else
    usage
fi


echo
echo "Benchmark complete."