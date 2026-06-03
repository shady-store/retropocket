#!/usr/bin/env bash
#
# Cross-compile retropocket-back for ARM64 (aarch64) / Cortex-A55
#
# Usage:
#   ./cross-compile-arm.sh                  # aarch64 Debug
#   ./cross-compile-arm.sh Release          # aarch64 Release
#   ./cross-compile-arm.sh Clean            # clean then build
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CMAKE_DIR="${SCRIPT_DIR}/cmake"
ROOT_DIR="${SCRIPT_DIR}"

BUILD_TYPE="Debug"
CLEAN=false

for arg in "$@"; do
    case "$arg" in
        Clean)     CLEAN=true;     ;;
        Release)   BUILD_TYPE="Release" ;;
        Debug)     BUILD_TYPE="Debug" ;;
        *)         echo "Unknown argument: $arg"; exit 1 ;;
    esac
done

TOOLCHAIN_FILE="${CMAKE_DIR}/aarch64-linux-gnu.cmake"
BUILD_DIR="${ROOT_DIR}/build-aarch64"
CC=aarch64-linux-gnu-gcc
CXX=aarch64-linux-gnu-g++

# ---------- Prerequisites check ----------
MISSING=()
for bin in "$CC" "$CXX" cmake make; do
    if ! command -v "$bin" &>/dev/null; then
        MISSING+=("$bin")
    fi
done

if [[ ${#MISSING[@]} -gt 0 ]]; then
    echo "ERROR: The following tools are not installed:"
    printf '  - %s\n' "${MISSING[@]}"
    echo ""
    echo "Install them with:"
    echo "  sudo dpkg --add-architecture arm64 && sudo apt update"
    echo "  sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu cmake build-essential"
    echo "  sudo apt install libssl-dev:arm64"
    exit 1
fi

# ---------- Clean if requested ----------
if [[ "$CLEAN" == true ]]; then
    echo "==> Cleaning previous build..."
    rm -rf "${BUILD_DIR}"
fi

mkdir -p "${BUILD_DIR}"

# ---------- Run CMake ----------
echo "==> Configuring for aarch64 / Cortex-A55 (build type: ${BUILD_TYPE})..."
(
    cd "${BUILD_DIR}"
    cmake -G "Unix Makefiles" \
        -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_FILE}" \
        -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
        "${SCRIPT_DIR}"
)

# ---------- Build ----------
echo "==> Building..."
(
    cd "${BUILD_DIR}"
    make -j"$(nproc)"
)

echo ""
echo "==> Build complete. Binary:"
echo "    ${BUILD_DIR}/retropocket-back"
echo ""
file "${BUILD_DIR}/retropocket-back"
