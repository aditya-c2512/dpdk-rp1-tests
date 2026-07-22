#!/bin/bash

set -e


WAIT_TIME=5

PACKET_SIZES=(64 128 256 512 1024 1500)


usage()
{
    echo "Usage:"
    echo
    echo "Single packet size:"
    echo "  $0 <runs> tx <ip> <port> <packet_size> <duration>"
    echo "  $0 <runs> rx <port> <packet_size> <duration>"
    echo
    echo "Packet size sweep:"
    echo "  $0 <runs> tx <ip> <port> sweep <duration>"
    echo "  $0 <runs> rx <port> sweep <duration>"
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



run_tx()
{
    IP=$1
    PORT=$2
    SIZE=$3
    DURATION=$4


    echo "================================"
    echo "TX Run"
    echo "Packet size: ${SIZE} bytes"
    echo "================================"


    for ((i=1;i<=RUNS;i++))
    do
        echo
        echo "TX iteration $i/$RUNS"


        $TX_BIN \
            $IP \
            $PORT \
            $SIZE \
            $DURATION


        echo "Completed TX run $i/$RUNS"


        if [ $i -lt $RUNS ]; then
            echo "Waiting ${WAIT_TIME}s..."
            sleep $WAIT_TIME
        fi

    done
}



run_rx()
{
    PORT=$1
    SIZE=$2
    DURATION=$3


    echo "================================"
    echo "RX Run"
    echo "Packet size: ${SIZE} bytes"
    echo "================================"


    for ((i=1;i<=RUNS;i++))
    do
        echo
        echo "RX iteration $i/$RUNS"


        $RX_BIN \
            $PORT \
            $SIZE \
            $DURATION


        echo "Completed RX run $i/$RUNS"


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
    SIZE=$5
    DURATION=$6



    if [ "$SIZE" == "sweep" ]; then

        echo "Starting TCP TX packet size sweep"


        for SIZE in "${PACKET_SIZES[@]}"
        do
            echo
            echo "################################"
            echo "Testing packet size ${SIZE}"
            echo "################################"


            run_tx \
                $IP \
                $PORT \
                $SIZE \
                $DURATION


            echo
            echo "Completed packet size ${SIZE}"

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


        echo "Starting TCP RX packet size sweep"


        for SIZE in "${PACKET_SIZES[@]}"
        do

            echo
            echo "################################"
            echo "Testing packet size ${SIZE}"
            echo "################################"


            run_rx \
                $PORT \
                $SIZE \
                $DURATION


            echo
            echo "Completed packet size ${SIZE}"

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
echo "TCP throughput benchmark complete."