#!/usr/bin/env bash

set -eou pipefail

# Set magic variables for current file & dir
__dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Color Constants
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_info() {
    echo -e "${BLUE}[ INFO ]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[ WARN ]${NC} $1"
}

print_error() {
    echo -e "${RED}[ ERROR ]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[ SUCCESS ]${NC} $1"
}

print_info "Starting build script..."

OUTPUT_DIR="build"
DEFAULT_TARGET="raytracer"

mkdir -p "$__dir"/"$OUTPUT_DIR"

cd "$__dir"/"$OUTPUT_DIR" || exit 255

print_info "Running cmake..."

cmake -S "$__dir" -B "$__dir"/"$OUTPUT_DIR" -DCMAKE_BUILD_TYPE=RELWITHDEBINFO

print_info "Building target $DEFAULT_TARGET..."

cmake --build $DEFAULT_TARGET

print_info "Building benchmarks..."

cmake --build raytracer-benchmark

print_success "Build succeeded"
