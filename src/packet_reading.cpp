#include <benchmark/benchmark.h>

#include "PcapFileDevice.h"
#include <fpcap/fpcap.hpp>
#include <pcap.h>

const static std::string inputFilePcap = "tracefiles/example.pcap";
const static std::string inputFilePcapNg = "tracefiles/example.pcapng";
const static std::string inputFilePcapNgZst = inputFilePcapNg + ".zst";
const static std::string inputFilePcapNgZstd = inputFilePcapNg + ".zstd";

static uint64_t bytesSum(const uint8_t* data, const size_t length) {
    uint64_t sum = 0;
    for (auto i = 0u; i < length; ++i) {
        benchmark::DoNotOptimize(sum += data[i]);
    }
    return sum;
}

static void bmFpcapPcap(benchmark::State& state) {
    for (auto _ : state) {
        fpcap::PacketReader reader(inputFilePcap, true);

        uint64_t packetCount = 0;
        fpcap::Packet packet;
        while (!reader.isExhausted()) {
            if (reader.nextPacket(packet)) {
                benchmark::DoNotOptimize(packetCount +=
                                         bytesSum(packet.data, packet.captureLength));
            }
        }

        benchmark::ClobberMemory();
    }
}

static void bmFpcapPcapFRead(benchmark::State& state) {
    for (auto _ : state) {
        fpcap::PacketReader reader(inputFilePcap, false);

        uint64_t packetCount = 0;
        fpcap::Packet packet;
        while (!reader.isExhausted()) {
            if (reader.nextPacket(packet)) {
                benchmark::DoNotOptimize(packetCount +=
                                         bytesSum(packet.data, packet.captureLength));
            }
        }

        benchmark::ClobberMemory();
    }
}

static void bmFpcapPcapNG(benchmark::State& state) {
    for (auto _ : state) {
        fpcap::PacketReader reader(inputFilePcapNg, true);

        uint64_t packetCount = 0;
        fpcap::Packet packet;
        while (!reader.isExhausted()) {
            if (reader.nextPacket(packet)) {
                benchmark::DoNotOptimize(packetCount +=
                                         bytesSum(packet.data, packet.captureLength));
            }
        }

        benchmark::ClobberMemory();
    }
}

static void bmFpcapPcapNGFRead(benchmark::State& state) {
    for (auto _ : state) {
        fpcap::PacketReader reader(inputFilePcapNg, false);

        uint64_t packetCount = 0;
        fpcap::Packet packet;
        while (!reader.isExhausted()) {
            if (reader.nextPacket(packet)) {
                benchmark::DoNotOptimize(packetCount +=
                                         bytesSum(packet.data, packet.captureLength));
            }
        }

        benchmark::ClobberMemory();
    }
}

static void bmFpcapPcapNGZst(benchmark::State& state) {
    for (auto _ : state) {
        fpcap::PacketReader reader(inputFilePcapNgZst);

        uint64_t packetCount = 0;
        fpcap::Packet packet;
        while (!reader.isExhausted()) {
            if (reader.nextPacket(packet)) {
                benchmark::DoNotOptimize(packetCount +=
                                         bytesSum(packet.data, packet.captureLength));
            }
        }

        benchmark::ClobberMemory();
    }
}

static void bmPcapPlusPlusPcap(benchmark::State& state) {
    for (auto _ : state) {
        pcpp::PcapFileReaderDevice reader(inputFilePcap);
        reader.open();

        uint64_t packetCount = 0;
        pcpp::RawPacket packet;
        while (reader.getNextPacket(packet)) {
            benchmark::DoNotOptimize(
                packetCount += bytesSum(packet.getRawData(), packet.getRawDataLen()));
        }

        reader.close();

        benchmark::ClobberMemory();
    }
}

static void bmPcapPlusPlusPcapNG(benchmark::State& state) {
    for (auto _ : state) {
        pcpp::PcapNgFileReaderDevice reader(inputFilePcapNg);
        reader.open();

        uint64_t packetCount = 0;
        pcpp::RawPacket packet;
        while (reader.getNextPacket(packet)) {
            benchmark::DoNotOptimize(
                packetCount += bytesSum(packet.getRawData(), packet.getRawDataLen()));
        }

        reader.close();

        benchmark::ClobberMemory();
    }
}

static void bmPcapPlusPlusPcapNGZstd(benchmark::State& state) {
    for (auto _ : state) {
        pcpp::PcapNgFileReaderDevice reader(inputFilePcapNgZstd);
        reader.open();

        uint64_t packetCount = 0;
        pcpp::RawPacket packet;
        while (reader.getNextPacket(packet)) {
            benchmark::DoNotOptimize(
                packetCount += bytesSum(packet.getRawData(), packet.getRawDataLen()));
        }

        reader.close();

        benchmark::ClobberMemory();
    }
}

static void bmLibpcapPcap(benchmark::State& state) {
    for (auto _ : state) {
        char errBuf[PCAP_ERRBUF_SIZE];
        pcap_t* pcapHandle = pcap_open_offline(inputFilePcap.c_str(), errBuf);

        uint64_t packetCount = 0;
        pcap_pkthdr header;
        const std::uint8_t* packet;
        while ((packet = pcap_next(pcapHandle, &header))) {
            benchmark::DoNotOptimize(packetCount += bytesSum(packet, header.caplen));
        }

        pcap_close(pcapHandle);

        benchmark::ClobberMemory();
    }
}

static void bmLibpcapPcapNG(benchmark::State& state) {
    for (auto _ : state) {
        char errBuf[PCAP_ERRBUF_SIZE];
        pcap_t* pcapHandle = pcap_open_offline(inputFilePcapNg.c_str(), errBuf);

        uint64_t packetCount = 0;
        pcap_pkthdr header;
        const std::uint8_t* packet;
        while ((packet = pcap_next(pcapHandle, &header))) {
            benchmark::DoNotOptimize(packetCount += bytesSum(packet, header.caplen));
        }

        pcap_close(pcapHandle);

        benchmark::ClobberMemory();
    }
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
