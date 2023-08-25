#include <benchmark/benchmark.h>

#include "fpcap/pcap/PcapReader.hpp"
#include "fpcap/pcapng/PcapNgReader.hpp"
#include "PcapFileDevice.h"
#include <pcap.h>

const static std::string inputFilePcap = "tracefiles/example.pcap";
const static std::string inputFilePcapNg = "tracefiles/example.pcapng";
const static std::string inputFilePcapNgZst = inputFilePcapNg + ".zst";
const static std::string inputFilePcapNgZstd = inputFilePcapNg + ".zstd";

static void bmFpcapPcap(benchmark::State& state) {
    fpcap::Packet packet;
    for (auto _ : state) {
        fpcap::MMPcapReader reader(inputFilePcap);

        uint64_t packetCount{0};
        while (!reader.isExhausted()) {
            if (reader.readNextPacket(packet)) {
                ++packetCount;
            }
        }

        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(packet);
}

static void bmFpcapPcapFRead(benchmark::State& state) {
    fpcap::Packet packet;
    for (auto _ : state) {
        fpcap::FReadPcapReader reader(inputFilePcap);

        uint64_t packetCount{0};
        while (!reader.isExhausted()) {
            if (reader.readNextPacket(packet)) {
                ++packetCount;
            }
        }

        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(packet);
}

static void bmFpcapPcapNG(benchmark::State& state) {
    fpcap::Packet packet;
    for (auto _ : state) {
        fpcap::MMPcapNgReader reader(inputFilePcapNg);

        uint64_t packetCount{0};
        while (!reader.isExhausted()) {
            if (reader.readNextPacket(packet)) {
                ++packetCount;
            }
        }

        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(packet);
}

static void bmFpcapPcapNGFRead(benchmark::State& state) {
    fpcap::Packet packet;
    for (auto _ : state) {
        fpcap::FReadPcapNgReader reader(inputFilePcapNg);

        uint64_t packetCount{0};
        while (!reader.isExhausted()) {
            if (reader.readNextPacket(packet)) {
                ++packetCount;
            }
        }

        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(packet);
}

static void bmFpcapPcapNGZst(benchmark::State& state) {
    fpcap::Packet packet;
    for (auto _ : state) {
        fpcap::ZstdPcapNgReader reader(inputFilePcapNgZst);

        uint64_t packetCount{0};
        while (!reader.isExhausted()) {
            if (reader.readNextPacket(packet)) {
                ++packetCount;
            }
        }

        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(packet);
}

static void bmPcapPlusPlusPcap(benchmark::State& state) {
    pcpp::RawPacket packet;
    for (auto _ : state) {
        pcpp::PcapFileReaderDevice reader(inputFilePcap);
        reader.open();

        uint64_t packetCount{0};
        while (reader.getNextPacket(packet)) {
            ++packetCount;
        }

        reader.close();

        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(packet);
}

static void bmPcapPlusPlusPcapNG(benchmark::State& state) {
    pcpp::RawPacket packet;
    for (auto _ : state) {
        pcpp::PcapNgFileReaderDevice reader(inputFilePcapNg);
        reader.open();

        uint64_t packetCount{0};
        while (reader.getNextPacket(packet)) {
            ++packetCount;
        }

        reader.close();

        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(packet);
}

static void bmPcapPlusPlusPcapNGZstd(benchmark::State& state) {
    pcpp::RawPacket packet;
    for (auto _ : state) {
        pcpp::PcapNgFileReaderDevice reader(inputFilePcapNgZstd);
        reader.open();

        uint64_t packetCount{0};
        while (reader.getNextPacket(packet)) {
            ++packetCount;
        }

        reader.close();

        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(packet);
}

static void bmLibpcapPcap(benchmark::State& state) {
    const std::uint8_t* packet;
    for (auto _ : state) {
        char errBuf[PCAP_ERRBUF_SIZE];
        pcap_t* pcapHandle = pcap_open_offline(inputFilePcap.c_str(), errBuf);

        uint64_t packetCount{0};
        pcap_pkthdr header;
        while ((packet = pcap_next(pcapHandle, &header))) {
            ++packetCount;
        }

        pcap_close(pcapHandle);

        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(packet);
}

static void bmLibpcapPcapNG(benchmark::State& state) {
    const std::uint8_t* packet;
    for (auto _ : state) {
        char errBuf[PCAP_ERRBUF_SIZE];
        pcap_t* pcapHandle = pcap_open_offline(inputFilePcapNg.c_str(), errBuf);

        uint64_t packetCount{0};
        pcap_pkthdr header;
        while ((packet = pcap_next(pcapHandle, &header))) {
            ++packetCount;
        }

        pcap_close(pcapHandle);

        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(packet);
}

BENCHMARK(bmFpcapPcap)->Name("fpcap (pcap)");
BENCHMARK(bmFpcapPcapFRead)->Name("fpcap-fread (pcap)");
BENCHMARK(bmFpcapPcapNG)->Name("fpcap (pcapng)");
BENCHMARK(bmFpcapPcapNGFRead)->Name("fpcap-fread (pcapng)");
BENCHMARK(bmFpcapPcapNGZst)->Name("fpcap (pcapng.zst)");
BENCHMARK(bmPcapPlusPlusPcap)->Name("PcapPlusPlus (pcap)");
BENCHMARK(bmPcapPlusPlusPcapNG)->Name("PcapPlusPlus (pcapng)");
BENCHMARK(bmPcapPlusPlusPcapNGZstd)->Name("PcapPlusPlus (pcapng.zstd)");
BENCHMARK(bmLibpcapPcap)->Name("libpcap (pcap)");
BENCHMARK(bmLibpcapPcapNG)->Name("libpcap (pcapng)");
