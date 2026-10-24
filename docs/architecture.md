# Architecture

- `src/bloch/compiler`: lexer, parser, AST, imports and semantic analysis.
- `src/bloch/runtime`: program evaluation, statevector simulation and tracing.
- `src/bloch/cli`: arguments, execution and output.
- `library/bloch/lang`: bundled standard library.
- `tests`: unit and CLI integration tests.
- `examples`: executable language examples.

The internal `bloch` namespace and source extension remain for compatibility during
the restart. User-facing commands and installation paths use `basis`.

CMake builds compiler, runtime and CLI libraries plus the `basis` executable.
CTest runs the test executable. `just check` also exercises examples and an installed
copy from a separate working directory.
