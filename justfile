set shell := ["bash", "-euo", "pipefail", "-c"]
set positional-arguments

default:
    @just --list

configure:
    cmake --preset dev

build: configure
    cmake --build --preset dev --parallel 2

test: build
    ctest --preset dev

run +args: build
    ./build/dev/bin/basis "$@"

format:
    bash scripts/format.sh

format-check:
    bash scripts/format.sh --check

check: format-check
    cmake --preset ci
    cmake --build --preset ci --parallel 2
    ctest --preset ci
    bash scripts/run_examples.sh build/ci/bin/basis
    bash scripts/check-install.sh build/ci

install *args:
    bash scripts/install.sh "$@"
