# Contributing to Basis

Bug reports, documentation improvements and focused code contributions are welcome.
Discuss significant language or architecture changes in an issue before implementing them.

## Set up

Install a C++20 compiler, CMake 3.28+, Ninja, Bash, just and clang-format 18.
Fork the repository if you do not have write access, then clone your fork.

```bash
git clone https://github.com/YOUR-USERNAME/basis.git
cd basis
git remote add upstream https://github.com/akshaysoftware/basis.git
git switch -c fix/describe-the-change
just build
```

## Development

`main` is the trunk. Create a short-lived branch for each change and open a PR to
`main`. Keep changes focused and add regression coverage for changed behaviour.

```bash
just format
just check
git diff --check
```

`just check` checks formatting, builds with warnings as errors, runs tests and
examples, and checks an installation from outside the source checkout.
See [the C++ style guide](docs/cpp-style.md) and [architecture](docs/architecture.md).

Use a Conventional Commit PR title, for example `fix: reject invalid gate operands`,
`feat: add seeded simulation` or `docs: clarify module imports`.
PRs are squash merged; their title becomes the commit title.

After your PR merges, delete the feature branch and update from `main`.
For substantial behaviour changes, update the relevant documentation and changelog.

## Reporting problems

Include the Basis version or commit, operating system, command, minimal program,
expected result and actual diagnostic. Follow [SECURITY.md](SECURITY.md) for private
security reports and [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) in project spaces.

Contributions are made under the project's Apache-2.0 license.
