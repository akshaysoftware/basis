# Basis

Basis (previously Bloch) is a modern, open-source programming language for quantum software.

This repository contains the current development version of the Basis language and compiler.

> **Status:** Basis is under active pre-1.0 development. Language syntax, compiler interfaces,
> package formats and internal representations may change between releases.

## Development

Basis requires:

- a C++20-compatible compiler
- CMake 3.28 or later
- Ninja

Configure the development build:

```bash
cmake --preset dev
```

Build:

```bash
cmake --build --preset dev
```

Test:

```bash
ctest --preset dev
```

Run:

```bash
./build/dev/bin/bloch
```

## Development conventions

C++ development conventions are documented in the
[Basis C++ Style Guide](docs/cpp-style.md).

## Project history

Basis was originally named "Bloch" and developed in the
[`bloch-labs/bloch-legacy`](https://github.com/bloch-labs/bloch-legacy)
repository.

The original implementation was published in the *Journal of Open Source Software* in 2026.

- Paper DOI: `10.21105/joss.09625`
- Archived software DOI: `10.5281/zenodo.18407424`

Development restarted in this repository in August 2026 with a refreshed compiler architecture.

## License

Basis is licensed under the Apache License 2.0.
