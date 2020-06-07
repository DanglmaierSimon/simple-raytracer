#!/usr/bin/env bash

set -o errexit
set -o pipefail

# Set magic variables for current file & dir
__dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Color Constants
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_info() {
    echo -e "${BLUE}$1${NC}"
}

print_warning() {
    echo -e "${YELLOW}$1${NC}"
}

print_error() {
    echo -e "${RED}$1${NC}"
}

print_success() {
    echo -e "${GREEN}$1${NC}"
}

print_info "Starting build script..."

OUTPUT_DIR="build"
DEFAULT_TARGET="raytracer"

mkdir -p "$__dir"/"$OUTPUT_DIR"

cd "$__dir"/"$OUTPUT_DIR" || exit 255

print_info "Running cmake..."

cmake .. -DCMAKE_BUILD_TYPE=RELWITHDEBINFO

print_info "Building target $DEFAULT_TARGET..."

make $DEFAULT_TARGET -j 4

print_success "==== BUILD SCRIPT SUCCESSFUL ===="
