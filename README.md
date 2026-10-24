# Basis

[![CI](https://github.com/akshaysoftware/basis/actions/workflows/ci.yml/badge.svg)](https://github.com/akshaysoftware/basis/actions/workflows/ci.yml)
[![License: Apache-2.0](https://img.shields.io/badge/License-Apache--2.0-blue.svg)](LICENSE)

Basis is an open-source, typed language for hybrid classical and quantum programs,
implemented in C++20 and maintained by [Akshay Pal](https://github.com/akshaysoftware).

The project is developing towards version 2.0. Language and compiler interfaces may
change. The current toolchain retains the `.bloch` source extension and `bloch.lang`
standard-library namespace from its predecessor.

## Build and run

Requirements: a C++20 compiler, CMake 3.28+, Ninja and Bash. Contributor commands also
use [just](https://just.systems) and clang-format 18.

```bash
git clone https://github.com/akshaysoftware/basis.git
cd basis
just build
just run examples/02_bell_state.bloch --shots=1000
just test
```

A small program looks like this:

```text
function main() -> void {
    @tracked qubit[2] q;
    h(q[0]);
    cx(q[0], q[1]);
    measure q;
}
```

## Install from source

From a checkout on macOS or Linux:

```bash
bash scripts/install.sh
export PATH="$HOME/.local/bin:$PATH"
basis --version
```

The installer builds and tests the current checkout, then installs the executable
and bundled standard library under `~/.local`. Pass an absolute prefix to change it:

```bash
bash scripts/install.sh /absolute/install/prefix
```

## Documentation and contributions

- [Documentation](docs/README.md)
- [Contributing](CONTRIBUTING.md)
- [Code of conduct](CODE_OF_CONDUCT.md)
- [Security reports](SECURITY.md)
- [Issues](https://github.com/akshaysoftware/basis/issues)

Run `just` to list development commands and `just check` before opening a PR.
Development uses short-lived feature branches and squash merges into `main`.

## Current limitations

The simulator's reset behaviour on entangled states requires correction. OpenQASM
2 output is an execution trace and does not preserve general measurement-dependent
control flow. Sampling does not yet expose a seed option. Treat these as development
limitations when evaluating results. See [known issues](docs/known-issues.md).

## History and citation

Basis continues the Bloch programming-language project in its original repository.
Bloch v1.0.3 accompanies the 2026 JOSS paper:

Pal, A. (2026). *Bloch: a strongly typed, hardware-agnostic, hybrid quantum
programming language*. Journal of Open Source Software, 11(117), 9625.
[Paper](https://doi.org/10.21105/joss.09625) ·
[Archived software](https://doi.org/10.5281/zenodo.18407424).

The paper describes the historical release. See [CITATION.cff](CITATION.cff).

## License

[Apache License 2.0](LICENSE).
