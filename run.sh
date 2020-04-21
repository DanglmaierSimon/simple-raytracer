#!/usr/bin/env bash

set -o errexit
set -o pipefail

./build.sh --release

cd build || exit -1

rm -f image.ppm

./raytracer >>image.ppm

gwenview image.ppm
