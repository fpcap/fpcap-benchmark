cmake_minimum_required(VERSION 3.18)

project(fpcap-benchmark VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Force Release build — this is a benchmark, other configs are meaningless
set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

include(FetchContent)

# fpcap
FetchContent_Declare(
    fpcap
    GIT_REPOSITORY https://github.com/fpcap/fpcap.git
    GIT_TAG        v0.2.0
)

# Google Benchmark
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_USE_BUNDLED_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.9.5
)

# PcapPlusPlus
FetchContent_Declare(
    PcapPlusPlus
    GIT_REPOSITORY https://github.com/seladb/PcapPlusPlus.git
    GIT_TAG        v25.05
)

FetchContent_MakeAvailable(fpcap benchmark PcapPlusPlus)

# Fix Npcap SDK SAL annotations not available in MinGW
if(MINGW)
    target_compile_definitions(Pcap++ PRIVATE _Post_invalid_=)
endif()

add_executable(fpcap_benchmark
    src/main.cpp
    src/packet_reading.cpp
)
target_link_libraries(fpcap_benchmark
    PRIVATE
        benchmark::benchmark
        fpcap::fpcap
        Pcap++
)
