# RP1 Networking Performance Testbench

## Overview

This repository contains a networking performance testbench designed to evaluate and compare different packet processing paths on Raspberry Pi platforms.

The primary objective of this project is to analyse the performance difference between:

1. **Traditional Linux networking stack**
    - TCP/IP socket path
    - UDP socket path
    - Standard kernel networking stack

2. **Kernel-bypass networking paths**
    - DPDK based packet processing
    - AF_XDP integration through DPDK
    - Custom Ethernet driver support for the Raspberry Pi RP1 Ethernet controller

The benchmark suite measures:

- Network throughput
- Packet processing rate
- Latency
- Packet drops
- CPU/networking overheads
- Performance variation across different packet sizes

The goal is to understand the overhead introduced by the traditional Linux networking stack and evaluate whether kernel-bypass technologies such as AF_XDP and DPDK can improve networking performance on embedded ARM platforms.

---

# Repository Structure

```
dpdk-rp1-tests/
│
├── benchmark/
│   ├── benchmark.c
│   └── benchmark.h
│
├── tcp/
│   ├── tcp_transport.c
│   ├── tcp_transport.h
│   └── TCP benchmark applications
│
├── udp/
│   ├── udp_transport.c
│   ├── udp_transport.h
│   └── UDP benchmark applications
│
├── latency/
│   ├── latency.c
│   └── latency.h
│
├── timer/
│   └── timer utilities
│
├── cadence/
│   └── Custom RP1 MACB Ethernet driver
│
├── dpdk/
│   └── DPDK based benchmark applications
│
└── CMakeLists.txt
```

---

# Test Objectives

## Throughput Benchmark

The throughput benchmark measures the maximum achievable bandwidth for different packet sizes.

Supported protocols:

- TCP
- UDP

The benchmark collects:

| Metric | Description |
|---|---|
| Throughput | Data rate in Mbps |
| Packets/sec | Packet processing rate |
| Packet count | Total packets processed |
| Bytes transferred | Total bytes transmitted/received |
| Errors | Transmission/reception errors |
| Runtime | Test duration |

Example:

```
TX packets=5287273 bytes=2833978328 rate=781.79 Mbps

TX 781.80 Mbps 182321.96 pps
```

---

## Latency Benchmark

The latency benchmark performs timestamp-based request/response measurements.

The client:

1. Creates a timestamped packet
2. Sends it to the server
3. Waits for the response
4. Calculates round-trip latency

Collected metrics:

- Minimum latency
- Average latency
- Maximum latency
- Latency histogram
- Percentile values:
    - p50
    - p90
    - p95
    - p99
    - p999

---

# Building the Testbench

## Dependencies

Install the required packages:

```bash
sudo apt update

sudo apt install \
    build-essential \
    cmake \
    git
```

---

## Clone Repository

```bash
git clone <repository-url>

cd dpdk-rp1-tests
```

---

## Build

Create a build directory:

```bash
mkdir build
cd build
```

Configure:

```bash
cmake ..
```

Compile:

```bash
make -j$(nproc)
```

The generated binaries will be available inside:

```
build/
```

---

# Running Linux Socket Benchmarks

The Linux socket benchmarks require two machines connected over Ethernet.

Example topology:

```
+----------------+
| Raspberry Pi 5 |
|    Sender      |
+----------------+
        |
        |
    Ethernet
        |
        |
+----------------+
| Raspberry Pi 5 |
|   Receiver     |
+----------------+
```

---

# UDP Throughput Test

## Receiver

Run:

```bash
./udp_throughput_receiver <port> <payload_size> <duration>
```

Example:

```bash
./udp_throughput_receiver 5000 512 30
```

---

## Sender

Run:

```bash
./udp_throughput_sender <receiver_ip> <port> <payload_size> <duration>
```

Example:

```bash
./udp_throughput_sender 192.168.100.1 5000 512 30
```

---

# UDP Latency Test

## Server

Start the latency server:

```bash
./udp_latency_server <port>
```

Example:

```bash
./udp_latency_server 5000
```

---

## Client

Run:

```bash
./udp_latency_client <server_ip> <port> <samples> <payload_size>
```

Example:

```bash
./udp_latency_client 192.168.100.1 5000 100000 64
```

---

# CSV Output Format

All benchmarks use a common CSV output format to allow direct comparison between networking methods.

Example:

```csv
timestamp,
test_type,
packet_size,
duration_seconds,
packets_sent,
packets_received,
bytes_sent,
bytes_received,
send_calls,
recv_calls,
partial_sends,
partial_recvs,
errors,
runtime_ns,
throughput_mbps,
packets_per_second
```

The output can be used for:

- Plotting throughput graphs
- Comparing networking implementations
- Statistical analysis
- Generating dissertation figures

---

# DPDK + AF_XDP Setup

This section describes the setup required to run the kernel-bypass networking benchmarks.

The AF_XDP + DPDK path requires:

- Custom RP1 MACB Ethernet driver
- Linux kernel XDP support
- DPDK installation
- Hugepage allocation
- AF_XDP PMD support

---

# 1. Build Linux Kernel With Custom MACB Driver

The Raspberry Pi kernel normally uses the upstream Cadence MACB Ethernet driver.

For this project, the driver has been replaced with a custom implementation supporting the RP1 Ethernet controller.

The custom driver is located at:

```
cadence/
```

The kernel should be rebuilt following the official Raspberry Pi kernel build instructions.

During kernel configuration, enable the required BPF and XDP options.

Required configuration options include:

```
CONFIG_XDP=y
CONFIG_BPF=y
CONFIG_BPF_SYSCALL=y
CONFIG_BPF_JIT=y
CONFIG_NET_CLS_BPF=y
CONFIG_NET_ACT_BPF=y
CONFIG_XDP_SOCKETS=y
```

Install the rebuilt kernel and reboot.

Verify the driver:

```bash
dmesg | grep macb
```

---

# 2. Install DPDK

Install DPDK and dependencies following the official DPDK documentation.

Required components:

- DPDK libraries
- Build tools
- Python dependencies

Verify installation:

```bash
dpdk-testpmd --version
```

---

# 3. Allocate Hugepages

DPDK requires hugepages for packet buffer allocation.

Allocate hugepages:

```bash
sudo sysctl -w vm.nr_hugepages=<number>
```

Verify:

```bash
grep Huge /proc/meminfo
```

---

# 4. Validate AF_XDP Using testpmd

Before running custom benchmarks, validate the AF_XDP path using DPDK testpmd.

Launch testpmd:

```bash
sudo ./build/app/dpdk-testpmd \
    --vdev=net_af_xdp0,iface=eth0 \
    -- \
    -i
```

Inside testpmd:

Start forwarding:

```
start
```

Check statistics:

```
show port stats all
```

Successful operation should show increasing:

```
RX packets
TX packets
```

---

# 5. Running DPDK Benchmarks

(DPDK benchmark applications are currently under development.)

The planned applications will provide:

- DPDK + AF_XDP throughput benchmark
- DPDK + AF_XDP latency benchmark
- Comparison against Linux socket benchmarks

The DPDK applications will use the same CSV output format to enable direct comparison.

---

# Future Work

Planned improvements:

- Complete DPDK benchmark applications
- Add AF_XDP copy-mode evaluation
- Add AF_XDP zero-copy evaluation
- Compare:

```
Linux sockets
        |
        |
AF_XDP copy mode
        |
        |
AF_XDP zero-copy
        |
        |
Native DPDK PMD
```

Additional measurements:

- CPU utilisation
- Memory overhead
- Interrupt behaviour
- Power consumption

The final objective is to quantify the trade-offs between traditional Linux networking and kernel-bypass packet processing on embedded ARM systems.