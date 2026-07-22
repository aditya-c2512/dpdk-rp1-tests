cmake_minimum_required(VERSION 3.10)
project(dpdk-rp1-tests C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -O3
)

include_directories(include)

add_library(common STATIC
        common/packet.c
        common/stats.c
        common/timer.c
        common/csv.c
        common/affinity.c
        common/logging.c
)

target_link_libraries(common m pthread)

add_executable(tcp_sender
        tcp/sender.c
        tcp/tcp_client.c
        tcp/tcp_server.c
        tcp/throughput.c
)

target_link_libraries(tcp_sender
        common
)

add_executable(tcp_receiver
        tcp/receiver.c
        tcp/tcp_client.c
        tcp/tcp_server.c
        tcp/throughput.c
)

target_link_libraries(tcp_receiver
        common
)

add_executable(latency_client
        tcp/latency_client.c
        tcp/tcp_client.c
        tcp/tcp_server.c
        tcp/throughput.c
)

target_link_libraries(latency_client
        common
)

add_executable(latency_server
        tcp/latency_server.c
        tcp/tcp_client.c
        tcp/tcp_server.c
        tcp/throughput.c
)

target_link_libraries(latency_server
        common
)

add_library(tcp STATIC
        tcp/tcp_client.c
        tcp/tcp_server.c
        tcp/throughput.c
)


target_link_libraries(tcp common)


target_link_libraries(tcp_sender tcp)

target_link_libraries(tcp_receiver tcp)
