#!/usr/bin/env bash

set -o errexit
set -o pipefail

cd build || exit -1

cmake ..

make -j4 raytracer