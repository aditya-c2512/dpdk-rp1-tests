# dpdk-rp1-tests
Test Applications to measure network performance (throughput and latency) comparing Linux TCP/IP stack vs DPDK (AF_XDP) on Raspberry Pi 5.

## Components
- `socket-bench`: Standard Linux UDP socket application for baseline measurements.
- `dpdk-bench`: DPDK-based application using the AF_XDP PMD for kernel bypassing.

## Requirements
- Raspberry Pi 5
- DPDK installed (`libdpdk-dev`)
- `libxdp` and `libbpf` for AF_XDP support.

## Building
```bash
mkdir build && cd build
cmake ..
make
```

## Running Baseline (Linux Stack)
On Receiver:
```bash
./socket-bench server
```
On Sender:
```bash
./socket-bench client <receiver_ip>
```

## Running DPDK (AF_XDP)
First, setup hugepages:
```bash
sudo ./setup_env.sh
```
Run the DPDK benchmark (example for eth0):
```bash
sudo ./dpdk-bench --vdev=net_af_xdp0,iface=eth0 --vdev=net_af_xdp1,iface=eth0 -- -p 3
```
*Note: The current `dpdk-bench` expects 2 ports for simple forwarding. For single-port testing, modifications to the source may be required depending on the test case.*

## Methodology
1. **Throughput**: Measured in Packets Per Second (PPS) and Mbps.
2. **Latency**: Can be measured by adding timestamps to payloads (to be implemented).
