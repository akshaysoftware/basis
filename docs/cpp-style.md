# Basis C++ Style Guide

This document defines the C++ conventions used by Basis.

The goal is not to create an exhaustive set of rules. Code should be readable, predictable,
safe and straightforward to maintain. Where this guide does not specify a convention, prefer
established modern C++ practices and consistency with the surrounding code.

## C++ version

Basis targets **C++20**.

Compiler-specific language extensions should not be used unless there is a clear portability
reason for doing so.

All supported code should compile with:

- GCC
- Clang
- AppleClang
- MSVC

Cross-platform support will be enforced progressively through CI.

## Formatting

`clang-format` is the source of truth for C++ formatting.

Do not manually format code in a way that conflicts with `.clang-format`.

Format the source tree with:

```bash
find src tests \
  -type f \
  \( -name '*.cpp' -o -name '*.hpp' -o -name '*.h' \) \
  -print0 \
  | xargs -0 clang-format -i
```

The configured maximum line length is 100 characters.

Use four spaces for indentation. Do not use tabs.

## Naming

### Files

Use `snake_case` for C++ filenames.

```text
semantic_analyser.cpp
semantic_analyser.hpp
module_loader.cpp
token.hpp
```

Header files should normally use `.hpp`.

### Namespaces

Namespaces use lowercase `snake_case`.

```cpp
namespace bloch {
namespace bloch::compiler {
namespace bloch::runtime {
```

Prefer nested namespace syntax:

```cpp
namespace bloch::compiler {

}  // namespace bloch::compiler
```

### Types

Classes, structs, enums and type aliases use `PascalCase`.

```cpp
class SemanticAnalyser;
struct SourceLocation;
enum class TokenType;
using SymbolTable = std::unordered_map<std::string, Symbol>;
```

### Functions

Functions and methods use `snake_case`.

```cpp
parse_expression();
resolve_symbol();
lower_function();
```

### Variables

Local variables and function parameters use `snake_case`.

```cpp
token_index
source_location
function_type
```

### Data members

Private data members use `snake_case` with a trailing underscore.

```cpp
class Parser {
private:
    std::vector<Token> tokens_;
    std::size_t current_;
};
```

Do not introduce Hungarian-style prefixes such as `m_`.

Existing legacy code using older conventions may be migrated incrementally.

### Constants

Compile-time constants use a `k` prefix followed by `PascalCase`.

```cpp
inline constexpr std::size_t kMaximumQubits = 64;
```

### Enum values

Scoped enum values use `PascalCase`.

```cpp
enum class TokenType {
    Identifier,
    IntegerLiteral,
    EndOfFile
};
```

Always prefer `enum class` over unscoped enums.

## Ownership and memory

Ownership should always be clear from the type.

Use `std::unique_ptr<T>` for exclusive ownership.

```cpp
std::unique_ptr<Expression>
```

Use references for required non-owning access.

```cpp
const SymbolTable& symbols
```

Use raw pointers only for explicitly non-owning or nullable relationships.

```cpp
const Symbol* parent = nullptr;
```

Use `std::shared_ptr<T>` only when ownership is genuinely shared.

Do not use `std::shared_ptr` simply because ownership is unclear.

Prefer RAII. Avoid direct use of `new`, `delete`, `malloc` or `free` unless implementing
genuinely low-level functionality where higher-level ownership types are unsuitable.

## Standard library

Prefer standard library types and algorithms over custom equivalents.

Use modern C++ vocabulary types where their semantics are appropriate:

```cpp
std::optional
std::variant
std::unique_ptr
std::string_view
std::span
std::filesystem::path
```

Do not use a type merely because it is modern. The type should accurately express the intended
ownership and lifetime semantics.

## `auto`

Use `auto` when it improves readability or avoids repeating an obvious or verbose type.

Good:

```cpp
auto token = lexer.next_token();
auto iterator = symbols.find(name);
```

Prefer an explicit type when the type communicates important information to the reader.

## `const`

Use `const` wherever an object is not intended to be modified.

Methods that do not mutate observable object state should be marked `const`.

```cpp
[[nodiscard]] bool has_errors() const;
```

## `[[nodiscard]]`

Use `[[nodiscard]]` when ignoring a return value is likely to represent a programming error.

Good candidates include:

- parse operations
- lookup operations
- validation operations
- predicates whose result must normally be considered

Do not add it mechanically to every function returning a value.

## `constexpr`

Use `constexpr` for values and functions that are meaningfully compile-time constructs.

Do not use it purely as decoration.

## `noexcept`

`noexcept` is a semantic guarantee.

Only mark functions `noexcept` when the implementation can genuinely uphold that guarantee.

## Headers

Headers must be self-contained.

A header should compile correctly when included without relying on another file having already
included one of its dependencies.

Include what you use.

Do not place `using namespace` directives in header files.

Prefer forward declarations when they meaningfully reduce coupling, but do not sacrifice
clarity merely to minimise includes.

Use:

```cpp
#pragma once
```

for include protection.

## Includes

Allow `clang-format` to order include groups.

Prefer this broad ordering:

1. the corresponding header for a `.cpp` file
2. C++ standard library headers
3. project headers

Example:

```cpp
#include "bloch/compiler/parser/parser.hpp"

#include <memory>
#include <string>
#include <vector>

#include "bloch/compiler/lexer/token.hpp"
#include "bloch/support/error/bloch_error.hpp"
```

## Control flow

Prefer explicit control flow over clever expressions.

Use braces for control-flow bodies, including single-statement bodies.

Preferred:

```cpp
if (tokens.empty()) {
    return;
}
```

rather than:

```cpp
if (tokens.empty())
    return;
```

Early returns are encouraged when they reduce nesting.

## Error handling

Compiler and runtime errors caused by Bloch Basis programs should use the diagnostic/error provided at `bloch/support/error/bloch_error.hpp`.

Exceptions may be used for exceptional failure paths.

Do not use exceptions for ordinary control flow.

Internal programming errors should fail clearly rather than silently continuing in an invalid
state.

## Comments

Comments should explain **why**, not restate **what** the code already says.

Avoid comments such as:

```cpp
// Increment i
++i;
```

Useful comments explain constraints, invariants, surprising behaviour or architectural
decisions.

Public APIs and non-obvious compiler components should receive concise documentation where
necessary.

## Functions

Prefer small functions with a single clear responsibility.

Avoid large functions that mix parsing, validation, transformation and output where those
responsibilities can be expressed independently.

Function parameters should make ownership expectations obvious through their types.

## Classes

Keep classes focused on one responsibility.

Prefer composition over inheritance unless inheritance models a genuine type relationship.

Make constructors `explicit` when a single-argument constructor should not provide an implicit
conversion.

```cpp
explicit Parser(std::vector<Token> tokens);
```

## CMake

Basis uses target-based CMake.

Prefer:

```cmake
target_compile_features(...)
target_compile_options(...)
target_include_directories(...)
target_link_libraries(...)
```

over global build configuration.

Avoid global commands such as:

```cmake
add_compile_options(...)
include_directories(...)
```

when the setting belongs to a particular target.

Each compiler component should declare its own dependencies.

## Legacy code

The initial compiler implementation was imported from `bloch-labs/bloch-legacy`.

Imported code does not necessarily comply with every convention in this guide.

Do not perform large mechanical rewrites solely for stylistic consistency unless there is a
clear engineering benefit.

New code should follow this guide, and legacy code should be migrated incrementally when it is
already being modified or when a focused refactoring provides clear value.