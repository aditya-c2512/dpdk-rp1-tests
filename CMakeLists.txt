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


add_library(common STATIC
        common/packet.c
        common/stats.c
        common/timer.c
        common/csv.c
        common/affinity.c
        common/logging.c
        tcp/latency.h
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



add_library(tcp STATIC
        tcp/tcp_client.c
        tcp/tcp_server.c
        tcp/throughput.c
        tcp/latency.c
)


target_include_directories(tcp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)


target_link_libraries(tcp
        PUBLIC
        common
)



add_executable(tcp_throughput_sender
        tcp/throughput_sender.c
)

target_link_libraries(tcp_throughput_sender tcp)



add_executable(tcp_throughput_receiver
        tcp/throughput_receiver.c
)

target_link_libraries(tcp_throughput_receiver tcp)



add_executable(tcp_latency_client
        tcp/latency_client.c
)

target_link_libraries(tcp_latency_client tcp)



add_executable(tcp_latency_server
        tcp/latency_server.c
)

target_link_libraries(tcp_latency_server tcp)