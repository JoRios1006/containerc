cmake_minimum_required(VERSION 3.14)
project(AlpineSetup NONE)

include(FetchContent)

FetchContent_Declare(
    alpine_minirootfs
    URL "https://dl-cdn.alpinelinux.org/alpine/v3.24/releases/x86_64/alpine-minirootfs-3.24.2-x86_64.tar.gz"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ALPINE"
)

FetchContent_MakeAvailable(alpine_minirootfs)
