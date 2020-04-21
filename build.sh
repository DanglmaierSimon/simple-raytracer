#!/usr/bin/env bash

set -o errexit
set -o pipefail

BUILD_TYPE="DEBUG"

while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
    --release)
        BUILD_TYPE="RELEASE"
        ;;
    --debug)
        BUILD_TYPE="DEBUG"
        ;;
    --profile)
        BUILD_TYPE="RELWITHDEBINFO"
        ;;
    esac
    shift
done

OUTPUT_DIR="build"
DEFAULT_TARGET="raytracer"

cd $OUTPUT_DIR || exit -1

cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE

make $DEFAULT_TARGET -j 4
