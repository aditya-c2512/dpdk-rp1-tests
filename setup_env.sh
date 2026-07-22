#!/bin/bash

# RP1 AF_XDP setup script

# Ensure eth0 is up
sudo ip link set dev eth0 up

# Check for AF_XDP support (needs kernel 4.18+, RPi5 kernel should be fine)
# For DPDK AF_XDP PMD, we need to ensure the interface is not used by the kernel stack
# if we want maximum performance, but AF_XDP can also work in SKB mode.

# Set hugepages (needed for DPDK)
echo 512 | sudo tee /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages

# Mount hugepages if not already mounted
if ! mount | grep -q hugetlbfs; then
    sudo mount -t hugetlbfs nodev /mnt/huge
fi

echo "Environment prepared for DPDK/AF_XDP testing."
echo "HUGEPAGES: $(cat /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages)"
