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

#include "bloch/compiler/lexer/lexer.hpp"

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "bloch/support/error/bloch_error.hpp"

namespace bloch::compiler {
namespace {

using Keyword = std::pair<std::string_view, TokenType>;

constexpr auto kKeywords = std::to_array<Keyword>({
    // Primitive types
    {"null", TokenType::Null},
    {"int", TokenType::Int},
    {"long", TokenType::Long},
    {"float", TokenType::Float},
    {"string", TokenType::String},
    {"char", TokenType::Char},
    {"qubit", TokenType::Qubit},
    {"bit", TokenType::Bit},
    {"boolean", TokenType::Boolean},

    // Boolean literals
    {"true", TokenType::True},
    {"false", TokenType::False},

    // Keywords
    {"void", TokenType::Void},
    {"function", TokenType::Function},
    {"return", TokenType::Return},
    {"if", TokenType::If},
    {"else", TokenType::Else},
    {"for", TokenType::For},
    {"while", TokenType::While},
    {"measure", TokenType::Measure},
    {"final", TokenType::Final},
    {"reset", TokenType::Reset},
    {"default", TokenType::Default},

    // Annotation values
    {"quantum", TokenType::Quantum},
    {"tracked", TokenType::Tracked},
    {"shots", TokenType::Shots},

    // Class system
    {"class", TokenType::Class},
    {"public", TokenType::Public},
    {"private", TokenType::Private},
    {"protected", TokenType::Protected},
    {"static", TokenType::Static},
    {"extends", TokenType::Extends},
    {"abstract", TokenType::Abstract},
    {"virtual", TokenType::Virtual},
    {"override", TokenType::Override},
    {"super", TokenType::Super},
    {"this", TokenType::This},
    {"import", TokenType::Import},
    {"package", TokenType::Package},
    {"new", TokenType::New},
    {"constructor", TokenType::Constructor},
    {"destructor", TokenType::Destructor},
    {"destroy", TokenType::Destroy},

    // Built-ins
    {"echo", TokenType::Echo},
});

[[nodiscard]] constexpr bool is_digit(char character) noexcept {
    return character >= '0' && character <= '9';
}

[[nodiscard]] constexpr bool is_alpha(char character) noexcept {
    return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') ||
           character == '_';
}

[[nodiscard]] constexpr bool is_alphanumeric(char character) noexcept {
    return is_alpha(character) || is_digit(character);
}

[[nodiscard]] constexpr bool is_whitespace(char character) noexcept {
    switch (character) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        case '\f':
        case '\v':
            return true;
        default:
            return false;
    }
}

[[nodiscard]] constexpr std::optional<TokenType> keyword_type(std::string_view text) noexcept {
    for (const auto& [keyword, type] : kKeywords) {
        if (text == keyword) {
            return type;
        }
    }
    return std::nullopt;
}

}  // namespace

using support::BlochError;
using support::ErrorCategory;

Lexer::Lexer(std::string_view source) noexcept : source_(source) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!is_at_end()) {
        skip_whitespace();
        if (is_at_end()) {
            break;
        }

        token_start_ = position_;
        token_line_ = line_;
        token_column_ = column_;
        tokens.push_back(scan_token());
    }

    tokens.push_back(Token{TokenType::Eof, "", line_, column_});
    return tokens;
}

bool Lexer::is_at_end() const noexcept { return position_ >= source_.size(); }

char Lexer::peek() const noexcept { return is_at_end() ? '\0' : source_[position_]; }

char Lexer::peek_next() const noexcept {
    return source_.size() - position_ > 1 ? source_[position_ + 1] : '\0';
}

char Lexer::advance() noexcept {
    const char character = source_[position_++];
    if (character == '\n') {
        ++line_;
        column_ = 1;
    } else {
        ++column_;
    }
    return character;
}

bool Lexer::match(char expected) noexcept {
    if (is_at_end() || source_[position_] != expected) {
        return false;
    }

    advance();
    return true;
}

void Lexer::skip_whitespace() noexcept {
    while (!is_at_end()) {
        if (is_whitespace(peek())) {
            advance();
            continue;
        }

        if (peek() == '/' && peek_next() == '/') {
            advance();
            advance();
            skip_comment();
            continue;
        }

        break;
    }
}

void Lexer::skip_comment() noexcept {
    while (!is_at_end() && peek() != '\n') {
        advance();
    }
}

void Lexer::report_error(std::string_view message) const {
    throw BlochError(ErrorCategory::Lexical, line_, column_, std::string(message));
}

Token Lexer::make_token(TokenType type) const {
    const auto lexeme = source_.substr(token_start_, position_ - token_start_);
    return Token{type, std::string(lexeme), token_line_, token_column_};
}

Token Lexer::scan_token() {
    const char character = advance();

    if (is_digit(character)) {
        return scan_number();
    }
    if (is_alpha(character)) {
        return scan_identifier_or_keyword();
    }

    switch (character) {
        case '=':
            return make_token(match('=') ? TokenType::EqualEqual : TokenType::Equals);
        case '!':
            return make_token(match('=') ? TokenType::BangEqual : TokenType::Bang);
        case '+':
            return make_token(match('+') ? TokenType::PlusPlus : TokenType::Plus);
        case '&':
            return make_token(match('&') ? TokenType::AmpersandAmpersand : TokenType::Ampersand);
        case '|':
            return make_token(match('|') ? TokenType::PipePipe : TokenType::Pipe);
        case '^':
            return make_token(TokenType::Caret);
        case '~':
            return make_token(TokenType::Tilde);
        case '-':
            if (match('>')) {
                return make_token(TokenType::Arrow);
            }
            return make_token(match('-') ? TokenType::MinusMinus : TokenType::Minus);
        case '*':
            return make_token(TokenType::Star);
        case '/':
            return make_token(TokenType::Slash);
        case '%':
            return make_token(TokenType::Percent);
        case '>':
            return make_token(match('=') ? TokenType::GreaterEqual : TokenType::Greater);
        case '<':
            return make_token(match('=') ? TokenType::LessEqual : TokenType::Less);
        case '?':
            return make_token(TokenType::Question);
        case ':':
            return make_token(TokenType::Colon);
        case '.':
            return make_token(TokenType::Dot);
        case ';':
            return make_token(TokenType::Semicolon);
        case ',':
            return make_token(TokenType::Comma);
        case '@':
            return make_token(TokenType::At);
        case '"':
            return scan_string();
        case '\'':
            return scan_char();
        case '(':
            return make_token(TokenType::LParen);
        case ')':
            return make_token(TokenType::RParen);
        case '{':
            return make_token(TokenType::LBrace);
        case '}':
            return make_token(TokenType::RBrace);
        case '[':
            return make_token(TokenType::LBracket);
        case ']':
            return make_token(TokenType::RBracket);
        default:
            return make_token(TokenType::Unknown);
    }
}

Token Lexer::scan_number() {
    while (is_digit(peek())) {
        advance();
    }

    if (peek() == '.') {
        advance();
        while (is_digit(peek())) {
            advance();
        }

        if (peek() == 'f') {
            advance();
            return make_token(TokenType::FloatLiteral);
        }

        report_error("float literals must end with 'f'");
    }

    if (peek() == 'f') {
        advance();
        return make_token(TokenType::FloatLiteral);
    }

    if (peek() == 'L') {
        advance();
        return make_token(TokenType::LongLiteral);
    }

    if (peek() == 'b') {
        const auto digits = source_.substr(token_start_, position_ - token_start_);
        if (digits != "0" && digits != "1") {
            report_error("bit literals must be 0b or 1b");
        }

        advance();
        return make_token(TokenType::BitLiteral);
    }

    return make_token(TokenType::IntegerLiteral);
}

Token Lexer::scan_identifier_or_keyword() {
    while (is_alphanumeric(peek())) {
        advance();
    }

    const auto identifier = source_.substr(token_start_, position_ - token_start_);
    const auto type = keyword_type(identifier).value_or(TokenType::Identifier);
    return make_token(type);
}

Token Lexer::scan_string() {
    while (!is_at_end() && peek() != '"') {
        advance();
    }

    if (is_at_end()) {
        report_error("unterminated string literal");
    }

    advance();
    return make_token(TokenType::StringLiteral);
}

Token Lexer::scan_char() {
    if (!is_at_end()) {
        advance();
    }

    if (peek() == '\'') {
        advance();
        return make_token(TokenType::CharLiteral);
    }

    report_error("unterminated char literal");
}

}  // namespace bloch::compiler
