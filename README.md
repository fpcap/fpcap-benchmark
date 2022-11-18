# mmpr-benchmark

Benchmark for mmpr - "A memory mapping PcapNg reader written in C++"

## Benchmark results

### 18.11.2022

MMPR: `d0cdf9a9940ffd5c3e65114335990854e2055887`

PcapPlusPlus: `a7326003891b3d82fabe2b84c98ccf5ba442b4f3`

File: `example.(pcap|pcapng|pcapng.zst|pcapng.zstd)`

```
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
mmpr (pcap)                  52257508 ns     10274420 ns           66
mmpr-fread (pcap)             8997999 ns      1278095 ns         1000
mmpr (pcapng)                61084662 ns     12835836 ns           55
mmpr-fread (pcapng)           9906805 ns      1468925 ns          424
mmpr (pcapng.zst)             8472729 ns      1926649 ns          353
PcapPlusPlus (pcap)          52835971 ns     11080767 ns           64
PcapPlusPlus (pcapng)        56371467 ns     12607466 ns           58
PcapPlusPlus (pcapng.zstd)    9406092 ns      2923663 ns          246
libpcap (pcap)               51586643 ns     10343324 ns           63
libpcap (pcapng)             53033134 ns     10650106 ns           66
```

### 18.11.2022

MMPR: `5ccaede821089c7e1b0979d9beaa58871ace8e63` (feature/move-code-from-headers-to-cpp)

PcapPlusPlus: `a7326003891b3d82fabe2b84c98ccf5ba442b4f3`

File: `example.(pcap|pcapng|pcapng.zst|pcapng.zstd)`

```
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
mmpr (pcap)                  53376839 ns     10940820 ns           56
mmpr-fread (pcap)             8031462 ns      1106749 ns          634
mmpr (pcapng)                52961521 ns     10787774 ns           65
mmpr-fread (pcapng)           8242220 ns      1132759 ns          622
mmpr (pcapng.zst)             8553374 ns      1928453 ns          381
PcapPlusPlus (pcap)          53317822 ns     11323495 ns           66
PcapPlusPlus (pcapng)        53371981 ns     11693164 ns           59
PcapPlusPlus (pcapng.zstd)    8941969 ns      2699589 ns          259
libpcap (pcap)               51376836 ns     10413706 ns           67
libpcap (pcapng)             52536349 ns     10585561 ns           66
```
