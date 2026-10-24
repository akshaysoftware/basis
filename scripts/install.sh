#!/usr/bin/env bash
set -euo pipefail

if (( $# > 1 )); then
    echo "Usage: $0 [absolute-install-prefix]" >&2
    exit 2
fi

prefix="${1:-$HOME/.local}"
case "$prefix" in
    /*) ;;
    *) echo "The installation prefix must be an absolute path." >&2; exit 2 ;;
esac

for tool in cmake ninja; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "Missing prerequisite: $tool" >&2
        exit 1
    }
done

cd "$(dirname "${BASH_SOURCE[0]}")/.."
cmake --preset release
cmake --build --preset release --parallel 2
ctest --preset release
cmake --install build/release --prefix "$prefix"

"$prefix/bin/basis" --version
printf '\nInstalled Basis to %s/bin/basis\nAdd %s/bin to your PATH.\n' "$prefix" "$prefix"
