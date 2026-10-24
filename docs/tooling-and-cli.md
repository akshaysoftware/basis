# CLI

```bash
basis [options] path/to/program.bloch
```

- `--help`: show options.
- `--version`: show the development version and commit.
- `--shots=N`: execute N shots and aggregate `@tracked` values.
- `--echo=auto|all|none`: control echo output.
- `--emit-qasm`: also print the OpenQASM 2 execution trace to stdout.

`@shots(N)` on `main` takes precedence over the CLI shot count. Successful runs
write a `.qasm` trace beside the input file. See [known issues](known-issues.md).

The bundled standard library is installed to `<prefix>/share/basis/library`.
Set `BASIS_STDLIB_PATH` to override its search root. The inherited package name
remains `bloch.lang`; for example, `import bloch.lang.Object;`.
