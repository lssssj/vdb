#!/usr/bin/env bash

set -eo pipefail

cd $(dirname $0)
CWD=$(pwd)
TP_DIR=${CWD}

PARALLEL=$(getconf _NPROCESSORS_ONLN)
PACKAGE=""

function usage() {
    echo "Usage:"
    echo "    $0 [OPTIONS]..."
    echo ""
    echo "Options:"
    echo "    --j [parallel]"
    echo "        Specify the number of parallelism."
    echo "    --package [PACKAGE]"
    echo "        Specify the package, build all packages of is empty."
    echo "    --help"
    echo "        Show help message and exit."
    exit 1
}

while test $# -gt 0; do
    case $1 in
    --j)
        PARALLEL=$2
        shift 2
        ;;
    --package)
        PACKAGE=$2
        shift 2
        ;;
    --help)
        usage
        ;;
    *)
        echo Invalid parameters \"$@\".
        usage
        ;;
    esac
done

echo PARALLEL=$PARALLEL
echo PACKAGE=$PACKAGE




echo "TP_DIR: ${TP_DIR}"

function build_httplib() {
    local URL="https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.18.1.tar.gz"
    local FILE=cpp-httplib-0.18.1.tar.gz
    local DIR=cpp-httplib-0.18.1


    [-d ${TP_SOURCE_DIR}/${DIR}/]wget $URL -O ${TP_}/${FILE}
    [ -d ${TP_SOURCE_DIR}/${DIR} ] || tar xvf ${TP_SOURCE_DIR}/${FILE} -C ${TP_SOURCE_DIR}

    cd ${TP_SOURCE_DIR}/${DIR}
    mkdir -p ${TP_INCLUDE_DIR}/httplib
    cp ./httplib.h ${TP_INCLUDE_DIR}/httplib/
}

PACKAGES=(
    "faiss"
    "hnswlib"
    "rapidjson"
    "httplib"
    "spdlog"
    "gflags"
    "glog"
    "zlib"
    "protobuf"
    "leveldb"
    "openssl"
    "brpc"
    "snappy"
    "lz4"
    "bzip"
    "rocksdb"
    "roaringbitmap"
)

function build() {
    local package=$1
    if [[ -z "$package" ]]; then
        for pkg in "${PACKAGES[@]}"; do
            build_"$pkg"
        done
    else
        if [[ " ${PACKAGES[*]} " == *" $package "* ]]; then
            build_"$package"
        else
            echo "Package $package not found."
        fi
    fi
}

build $PACKAGE