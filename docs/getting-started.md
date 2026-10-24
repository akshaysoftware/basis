# Getting started

Follow the [README](../README.md) to build or install Basis from source.

```bash
just run examples/01_hadamard.bloch --shots=1000
just run examples/02_bell_state.bloch --shots=1000
```

Use `just run --help` for CLI options. The files in `examples/` demonstrate the
current language; `10_destructor_runtime_error.bloch` intentionally reports an error.
Read [known issues](known-issues.md) before relying on simulator or export results.
