cmake_minimum_required(VERSION 3.14)
project(AlpineSetup C)

# Suppress policy CMP0135 warning
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

FetchContent_Declare(
    alpine_minirootfs
    URL "https://dl-cdn.alpinelinux.org/alpine/v3.24/releases/x86_64/alpine-minirootfs-3.24.2-x86_64.tar.gz"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ALPINE"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(alpine_minirootfs)

add_executable(main main.c)
