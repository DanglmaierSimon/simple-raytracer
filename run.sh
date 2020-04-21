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

print_info "Building project in release mode..."

"$__dir/build.sh" --release

cd "$__dir/build" || exit -1

print_info "Removing old image..."

rm -f image.ppm

print_info "Generating image..."

"$__dir/build/raytracer"

print_success "Image created successfully!"

gwenview image.ppm &
