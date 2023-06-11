#!/usr/bin/env bash

set -eou pipefail

IMG_FILE=image.ppm

echo "Building project in debug mode..."
cargo build

echo "Building project in release mode..."
cargo build --release

echo "Removing old image..."

rm -f $IMG_FILE

echo "Generating image..."

./target/release/simple-raytracer > $IMG_FILE

echo "Image created successfully!"

gwenview $IMG_FILE
