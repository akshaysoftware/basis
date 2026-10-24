#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."

formatter="${BASIS_CLANG_FORMAT:-clang-format-18}"
if ! command -v "$formatter" >/dev/null 2>&1; then
    formatter=clang-format
fi

version="$("$formatter" --version)"
[[ "$version" == *"version 18."* ]] || {
    echo "clang-format 18 is required; set BASIS_CLANG_FORMAT to its path." >&2
    exit 1
}

case "${1:-}" in
    '') options=(-i) ;;
    --check) options=(--dry-run --Werror) ;;
    *) echo "Usage: $0 [--check]" >&2; exit 2 ;;
esac

find src tests -type f \( -name '*.cpp' -o -name '*.hpp' -o -name '*.h' \) -print0 |
    xargs -0 "$formatter" "${options[@]}"
