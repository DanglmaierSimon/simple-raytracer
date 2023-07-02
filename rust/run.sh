#!/usr/bin/env bash

set -eou pipefail

echo "Building project in debug mode..."
cargo build

echo "Building project in release mode..."
cargo build --release

echo "Removing old image..."

echo "Generating image..."

./target/release/simple-raytracer

echo "Image created successfully!"

gwenview "image.ppm"
