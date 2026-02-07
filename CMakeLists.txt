cmake_minimum_required(VERSION 3.18)

project(fpcap-benchmark VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)

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
if(WIN32 AND NOT PCAP_ROOT)
    FetchContent_Declare(
        npcap_sdk
        URL https://npcap.com/dist/npcap-sdk-1.16.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(npcap_sdk)
    FetchContent_GetProperties(npcap_sdk)
    set(PCAP_ROOT "${npcap_sdk_SOURCE_DIR}" CACHE PATH "Npcap SDK path" FORCE)
endif()
if(MINGW)
    add_compile_definitions(_Post_invalid_=)
endif()
FetchContent_Declare(
    PcapPlusPlus
    GIT_REPOSITORY https://github.com/seladb/PcapPlusPlus.git
    GIT_TAG        v25.05
)

FetchContent_MakeAvailable(fpcap benchmark PcapPlusPlus)

add_executable(fpcap_benchmark
    src/main.cpp
    src/packet_reading.cpp
)
target_compile_options(fpcap_benchmark PRIVATE -O3)
target_link_libraries(fpcap_benchmark
    PRIVATE
        benchmark::benchmark
        fpcap::fpcap
        Pcap++
)
