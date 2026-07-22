cmake_minimum_required(VERSION 3.20)

project(dpdk-rp1-tests C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -O3
)


#
# Common library
#

add_library(common STATIC
        common/packet.c
        common/stats.c
        common/timer.c
        common/csv.c
        common/affinity.c
        common/logging.c
)


target_include_directories(common
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)


target_link_libraries(common
        PUBLIC
        m
        pthread
)



#
# TCP library
#

add_library(tcp STATIC
        tcp/tcp_client.c
        tcp/tcp_server.c
        tcp/tcp_transport.c
        tcp/throughput.c
        tcp/latency.c
)


target_include_directories(tcp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/tcp
)


target_link_libraries(tcp
        PUBLIC
        common
)



#
# TCP Throughput
#

add_executable(tcp_throughput_sender
        tcp/throughput_sender.c
)

target_link_libraries(tcp_throughput_sender
        tcp
)



add_executable(tcp_throughput_receiver
        tcp/throughput_receiver.c
)

target_link_libraries(tcp_throughput_receiver
        tcp
)



#
# TCP Latency
#

add_executable(tcp_latency_client
        tcp/latency_client.c
)

target_link_libraries(tcp_latency_client
        tcp
)



add_executable(tcp_latency_server
        tcp/latency_server.c
)

target_link_libraries(tcp_latency_server
        tcp
)



#
# UDP library
#

add_library(udp STATIC
        udp/udp_transport.c
        udp/throughput.c
)


target_include_directories(udp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/udp
)


target_link_libraries(udp
        PUBLIC
        common
)



#
# UDP Throughput
#

add_executable(udp_throughput_sender
        udp/throughput_sender.c
)

target_link_libraries(udp_throughput_sender
        udp
)



add_executable(udp_throughput_receiver
        udp/throughput_receiver.c
)

target_link_libraries(udp_throughput_receiver
        udp
)