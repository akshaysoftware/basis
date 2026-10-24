#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."

build_dir="${1:-build/ci}"
scratch="$(mktemp -d)"
trap 'rm -rf "$scratch"' EXIT

cmake --install "$build_dir" --prefix "$scratch/prefix"
mkdir "$scratch/work"

cat > "$scratch/work/smoke.bloch" <<'BLOCH'
function main() -> void {
    qubit q;
    x(q);
    bit result = measure q;
    echo(result);
    Object object = new Object();
    echo(object.toString());
}
BLOCH

expected_version="$(cat version.txt)"
(
    cd "$scratch/work"
    unset BASIS_STDLIB_PATH
    export PATH="$scratch/prefix/bin:$PATH"

    version="$(basis --version)"
    [[ "$version" == "$expected_version"* ]]

    output="$(basis smoke.bloch)"
    [[ "$output" == $'1\nbloch.lang.Object' ]]
)

echo "Installed executable and bundled standard library passed the smoke test."
