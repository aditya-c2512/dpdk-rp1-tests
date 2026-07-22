#!/bin/bash

set -e


WAIT_TIME=5


usage()
{
    echo "Usage:"
    echo
    echo "TX:"
    echo "  $0 <runs> tx <ip> <port> <samples>"
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


CLIENT_BIN="${SCRIPT_DIR}/latency_client"
SERVER_BIN="${SCRIPT_DIR}/latency_server"



if [ "$MODE" == "tx" ]; then

    if [ $# -ne 5 ]; then
        usage
    fi


    IP=$3
    PORT=$4
    SAMPLES=$5


    echo "================================"
    echo "TCP Latency Client"
    echo "================================"
    echo "Runs: $RUNS"
    echo "Destination: $IP:$PORT"
    echo "Samples/run: $SAMPLES"
    echo



    for ((i=1;i<=RUNS;i++))
    do

        echo "================================"
        echo "Latency TX Run $i/$RUNS"
        echo "================================"



        $CLIENT_BIN \
            $IP \
            $PORT \
            $SAMPLES



        echo
        echo "Completed latency TX run $i/$RUNS"



        if [ $i -lt $RUNS ]; then
            echo "Waiting ${WAIT_TIME}s..."
            sleep $WAIT_TIME
        fi

    done



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