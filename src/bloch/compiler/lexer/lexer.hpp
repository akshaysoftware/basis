// Copyright 2025-2026 Akshay Pal (https://github.com/akshaysoftware)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

#include "bloch/compiler/lexer/token.hpp"

namespace bloch::compiler {

// Converts source text into a flat token stream in a single pass. The source must outlive the
// lexer because it is retained as a non-owning view.
class Lexer {
   public:
    explicit Lexer(std::string_view source) noexcept;

    [[nodiscard]] std::vector<Token> tokenize();

   private:
    std::string_view source_;
    std::size_t position_ = 0;
    std::size_t token_start_ = 0;
    int line_ = 1;
    int column_ = 1;
    int token_line_ = 1;
    int token_column_ = 1;

    [[nodiscard]] bool is_at_end() const noexcept;
    [[nodiscard]] char peek() const noexcept;
    [[nodiscard]] char peek_next() const noexcept;
    char advance() noexcept;
    [[nodiscard]] bool match(char expected) noexcept;

    void skip_whitespace() noexcept;
    void skip_comment() noexcept;
    [[noreturn]] void report_error(std::string_view message) const;

    [[nodiscard]] Token make_token(TokenType type) const;
    [[nodiscard]] Token scan_token();
    [[nodiscard]] Token scan_number();
    [[nodiscard]] Token scan_identifier_or_keyword();
    [[nodiscard]] Token scan_string();
    [[nodiscard]] Token scan_char();
};

}  // namespace bloch::compiler
