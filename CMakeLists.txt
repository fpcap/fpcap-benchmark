cmake_minimum_required(VERSION 3.18)

project(fpcap-benchmark VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Memory-Mapping PCAP Reader (MMPR)
add_subdirectory(libs/fpcap)

# Google benchmark
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_USE_BUNDLED_GTEST OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/benchmark EXCLUDE_FROM_ALL)

# PcapPlusPlus
add_subdirectory(libs/PcapPlusPlus)

add_executable(fpcap_benchmark
    src/main.cpp
    src/packet_reading.cpp
)
target_compile_features(fpcap_benchmark PRIVATE cxx_std_11)
target_link_libraries(fpcap_benchmark
    PRIVATE
        benchmark::benchmark
        fpcap::fpcap
        PcapPP
        pcap
)
