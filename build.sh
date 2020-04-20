#!/usr/bin/env bash

set -o errexit
set -o pipefail

cd build || exit -1

cmake ..

make raytracer

rm -f image.ppm

./raytracer >>image.ppm

gwenview image.ppm
