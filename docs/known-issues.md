# Known development issues

- Resetting an entangled qubit can incorrectly bias the state of other qubits.
- Two-qubit gates currently accept identical control and target operands.
- OpenQASM 2 output records one executed path, including the final shot's path in
  multi-shot runs. It does not preserve general measurement-dependent control flow.
- Sampling has no public seed option.
- Malformed `--shots` values need stricter validation and can terminate the CLI.

These behaviours require focused fixes with regression coverage before a stable v2
release. The unmerged QASM3 work remains archived on the restart repository's feature
branch until its export semantics are reviewed.
