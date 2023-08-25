# fpcap-benchmark

Benchmark for fpcap - "A fast and lightweight PCAP file reading library"

## Benchmark results

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
