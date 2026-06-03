# CMake toolchain file for cross-compiling to ARM 64-bit (aarch64)
# Target: Cortex-A55 (ARMv8-A)
#
# Usage:
#   cmake -DCMAKE_TOOLCHAIN_FILE=cmake/aarch64-linux-gnu.cmake ..
#
# Prerequisites:
#   sudo dpkg --add-architecture arm64 && sudo apt update
#   sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu \
#     libssl-dev:arm64
#
# Note: cpp-httplib (used by Update.hpp) requires 64-bit platforms,
# so aarch64 is the correct target instead of arm32 (armhf).

# ---------------------
# Target system
# ---------------------
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# ---------------------
# Search paths
# ---------------------
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# ---------------------
# Compiler / linker flags — Cortex-A55 specific
# ---------------------
#
# Cortex-A55 (ARMv8.0-A) in AArch64 mode exposes:
#   - full ARMv8-A ISA (64-bit)
#   - FP + Advanced SIMD (NEON in AArch64)
#   - CRC32 instructions
#   - LUCP/RCPC (late/early vector register allocation)
#
set(CMAKE_C_FLAGS_INIT   "-march=armv8-a+crc+crypto -mtune=cortex-a55")
set(CMAKE_CXX_FLAGS_INIT "-march=armv8-a+crc+crypto -mtune=cortex-a55")

# ---------------------
# OpenSSL hint
# ---------------------
if(NOT DEFINED CROSS_OPENSSL_ROOT)
    set(CROSS_OPENSSL_ROOT /usr/lib/aarch64-linux-gnu CACHE PATH
        "Root path where cross-compiled OpenSSL is installed")
endif()

list(APPEND CMAKE_PREFIX_PATH "${CROSS_OPENSSL_ROOT}")
