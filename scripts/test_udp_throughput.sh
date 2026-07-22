#!/bin/bash

set -e


WAIT_TIME=5


PACKET_SIZES=(
    64
    128
    256
    512
    1024
    1400
)



usage()
{
    echo "Usage:"
    echo
    echo "Single size:"
    echo "  $0 <runs> tx <ip> <port> <packet_size> <duration>"
    echo "  $0 <runs> rx <port> <packet_size> <duration>"
    echo
    echo "Sweep:"
    echo "  $0 <runs> tx <ip> <port> sweep <duration>"
    echo "  $0 <runs> rx <port> sweep <duration>"
    exit 1
}



if [ $# -lt 2 ]; then
    usage
fi



RUNS=$1
MODE=$2


SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

BUILD_DIR="${SCRIPT_DIR}/../build"

TX_BIN="${BUILD_DIR}/udp_throughput_sender"
RX_BIN="${BUILD_DIR}/udp_throughput_receiver"



run_tx()
{
    IP=$1
    PORT=$2
    SIZE=$3
    DURATION=$4


    for ((i=1;i<=RUNS;i++))
    do
        echo "================================"
        echo "UDP TX Run $i/$RUNS"
        echo "Packet size: $SIZE"
        echo "================================"


        $TX_BIN \
            $IP \
            $PORT \
            $SIZE \
            $DURATION


        echo
        echo "Completed TX run $i/$RUNS"


        if [ $i -lt $RUNS ]; then
            sleep $WAIT_TIME
        fi

    done
}



run_rx()
{
    PORT=$1
    SIZE=$2
    DURATION=$3


    for ((i=1;i<=RUNS;i++))
    do
        echo "================================"
        echo "UDP RX Run $i/$RUNS"
        echo "Packet size: $SIZE"
        echo "================================"


        $RX_BIN \
            $PORT \
            $SIZE \
            $DURATION


        echo
        echo "Completed RX run $i/$RUNS"


        if [ $i -lt $RUNS ]; then
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
    SIZE=$5
    DURATION=$6



    if [ "$SIZE" == "sweep" ]; then


        echo "================================"
        echo "UDP TX Packet Sweep"
        echo "================================"


        for SIZE in "${PACKET_SIZES[@]}"
        do

            echo
            echo "Starting packet size ${SIZE}"

            run_tx \
                $IP \
                $PORT \
                $SIZE \
                $DURATION


            echo "Waiting before next packet size..."
            sleep $WAIT_TIME

        done


    else

        run_tx \
            $IP \
            $PORT \
            $SIZE \
            $DURATION

    fi



elif [ "$MODE" == "rx" ]; then


    if [ $# -ne 5 ]; then
        usage
    fi


    PORT=$3
    SIZE=$4
    DURATION=$5



    if [ "$SIZE" == "sweep" ]; then


        echo "================================"
        echo "UDP RX Packet Sweep"
        echo "================================"


        for SIZE in "${PACKET_SIZES[@]}"
        do

            echo
            echo "Starting packet size ${SIZE}"

            run_rx \
                $PORT \
                $SIZE \
                $DURATION


            echo "Waiting before next packet size..."
            sleep $WAIT_TIME

        done


    else

        run_rx \
            $PORT \
            $SIZE \
            $DURATION

    fi



else

    usage

fi


echo
echo "UDP throughput benchmark complete."