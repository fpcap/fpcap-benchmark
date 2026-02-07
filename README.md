# fpcap-benchmark

Benchmark for fpcap - "A fast and lightweight PCAP file reading library"

## Build

### Windows

Download Npcap SDK and Installer from https://npcap.com/#download

Run the Npcap installer and unpack the SDK and add the directory to your PATH.

Get zstd, unpack and add it to PATH https://github.com/facebook/zstd/releases/tag/v1.5.7

Run CMake with:

```
-DPCAP_ROOT=C:\npcap-sdk-1.16 -DZSTD_ROOT=C:\zstd-v1.5.7-win64 -DZSTD_LIBRARY=C:\zstd-v1.5.7-win64\dll\libzstd.dll.a
```

## Disabling CPU Frequency Scaling

If you see this error:

```
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
```

you might want to disable the CPU frequency scaling while running the benchmark. Exactly how to do this depends on the
Linux distribution, desktop environment, and installed programs.

One simple option is to use the `cpupower` program to change the performance governor to "performance". This tool is
maintained along with the Linux kernel and provided by your distribution.

It must be run as root, like this:

```bash
sudo cpupower frequency-set --governor performance
```

After this you can verify that all CPUs are using the performance governor by running this command:

```bash
cpupower frequency-info -o proc
```

The benchmarks you subsequently run will have less variance.

Note that changing the governor in this way will not persist across reboots. To set the governor back, run the first
command again with the governor your system usually runs with, which varies.

If you find yourself doing this often, there are probably better options than running the commands above. Some
approaches allow you to do this without root access, or by using a GUI, etc. The Arch
Wiki [Cpu frequency scaling](https://wiki.archlinux.org/title/CPU_frequency_scaling) page is a good place to start
looking for options.

## Benchmark results

### 07.02.2026

FPCAP: v0.2.0

PcapPlusPlus: v25.05

File: `example.(pcap|pcapng|pcapng.zst|pcapng.zstd)`

OS: Windows 11 Pro

CPU: 12th Gen Intel(R) Core(TM) i9-12900KS (3.40 GHz)

RAM: 64GB DDR5

Raw benchmark output:

```
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
fpcap (pcap)                  3021247 ns      2780720 ns          236
fpcap-fread (pcap)            3472708 ns      3125000 ns          195
fpcap (pcapng)                2881581 ns      2722538 ns          264
fpcap-fread (pcapng)          3443721 ns      3446691 ns          204
fpcap (pcapng.zst)            4782850 ns      4768669 ns          154
PcapPlusPlus (pcap)           4199388 ns      4269622 ns          172
PcapPlusPlus (pcapng)         5477204 ns      5301339 ns          112
PcapPlusPlus (pcapng.zstd)    5222558 ns      5022321 ns          112
libpcap (pcap)                3907070 ns      3927139 ns          187
libpcap (pcapng)              4211563 ns      4277235 ns          179
```

Iterations (higher is better):

```                                                                                                                                                                                                                                                                                                               
.pcap                                
  fpcap          █████████████████████████████████████████████       236
  fpcap-fread    █████████████████████████████████████               195
  libpcap        ███████████████████████████████████                 187
  PcapPlusPlus   █████████████████████████████████                   172

.pcapng
  fpcap          ██████████████████████████████████████████████████  264
  fpcap-fread    ███████████████████████████████████████             204
  libpcap        ██████████████████████████████████                  179
  PcapPlusPlus   █████████████████████                               112

.pcapng.zst(d)
  fpcap          █████████████████████████████████████████████████   154
  PcapPlusPlus   █████████████████████                               112
```

### 18.11.2022

FPCAP: `d0cdf9a9940ffd5c3e65114335990854e2055887`

PcapPlusPlus: `a7326003891b3d82fabe2b84c98ccf5ba442b4f3`

File: `example.(pcap|pcapng|pcapng.zst|pcapng.zstd)`

```
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
fpcap (pcap)                 65178523 ns     14507273 ns           51
fpcap-fread (pcap)            9903993 ns      1431813 ns          507
fpcap (pcapng)               56407792 ns     11436369 ns           61
fpcap-fread (pcapng)          8264006 ns      1106907 ns          614
fpcap (pcapng.zst)            8866711 ns      2020582 ns          372
PcapPlusPlus (pcap)          53472271 ns     11187879 ns           56
PcapPlusPlus (pcapng)        58684604 ns     13140057 ns           61
PcapPlusPlus (pcapng.zstd)    9014957 ns      2742154 ns          241
libpcap (pcap)               52502548 ns     10682387 ns           68
libpcap (pcapng)             57018837 ns     11996940 ns           60
```

### 18.11.2022

FPCAP: `5ccaede821089c7e1b0979d9beaa58871ace8e63` (feature/move-code-from-headers-to-cpp)

PcapPlusPlus: `a7326003891b3d82fabe2b84c98ccf5ba442b4f3`

File: `example.(pcap|pcapng|pcapng.zst|pcapng.zstd)`

```
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
fpcap (pcap)                 53376839 ns     10940820 ns           56
fpcap-fread (pcap)            8031462 ns      1106749 ns          634
fpcap (pcapng)               52961521 ns     10787774 ns           65
fpcap-fread (pcapng)          8242220 ns      1132759 ns          622
fpcap (pcapng.zst)            8553374 ns      1928453 ns          381
PcapPlusPlus (pcap)          53317822 ns     11323495 ns           66
PcapPlusPlus (pcapng)        53371981 ns     11693164 ns           59
PcapPlusPlus (pcapng.zstd)    8941969 ns      2699589 ns          259
libpcap (pcap)               51376836 ns     10413706 ns           67
libpcap (pcapng)             52536349 ns     10585561 ns           66
```

### 24.11.2022

FPCAP: `539168017ce54b6a84cf4eab8a646fb4b88ecaf7` (feature/move-code-from-headers-to-cpp)

PcapPlusPlus: `a7326003891b3d82fabe2b84c98ccf5ba442b4f3`

File: `example.(pcap|pcapng|pcapng.zst|pcapng.zstd)`

```
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
fpcap (pcap)                 55305059 ns     11354455 ns           65
fpcap-fread (pcap)            8729938 ns      1237597 ns          585
fpcap (pcapng)               54033123 ns     10694745 ns           67
fpcap-fread (pcapng)          8137800 ns      1074424 ns          612
fpcap (pcapng.zst)            8076516 ns      1804078 ns          385
PcapPlusPlus (pcap)          51511306 ns     10483894 ns           62
PcapPlusPlus (pcapng)        53430211 ns     11567145 ns           60
PcapPlusPlus (pcapng.zstd)    9181265 ns      2826843 ns          253
libpcap (pcap)               51366550 ns     10124561 ns           69
libpcap (pcapng)             52027888 ns     10341321 ns           68
```

### 13.10.2024

FPCAP: `d1f2adcd5db58d50acc3cf94a87b2ae309ddead9` (Removed need for dynamically allocated readers)

PcapPlusPlus: `a7326003891b3d82fabe2b84c98ccf5ba442b4f3`

File: `example.(pcap|pcapng|pcapng.zst|pcapng.zstd)`

```
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
fpcap (pcap)                 55124758 ns      2621275 ns          100
fpcap-fread (pcap)           10461908 ns      1150488 ns          629
fpcap (pcapng)               56311348 ns      2759284 ns          100
fpcap-fread (pcapng)         10601373 ns      1132879 ns          620
fpcap (pcapng.zst)           10199095 ns      1820104 ns          382
PcapPlusPlus (pcap)          53240374 ns      3304145 ns          100
PcapPlusPlus (pcapng)        55926923 ns      4252597 ns          100
PcapPlusPlus (pcapng.zstd)   10338093 ns      2968186 ns          239
libpcap (pcap)               57044627 ns      2908854 ns          100
libpcap (pcapng)             55424018 ns      2875435 ns          100
```