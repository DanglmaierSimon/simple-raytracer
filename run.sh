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

print_info "Building project in profile mode..."

"$__dir/build.sh"

cd "$__dir/build" || exit 255

print_info "Removing old image..."

rm -f image.ppm

print_info "Generating image..."

"$__dir/build/bin/raytracer"

print_success "Image created successfully!"

gwenview image.ppm
