#pragma once

#ifndef STATLOG_FORMATTER_PATTERN_INCLUDED
#define STATLOG_FORMATTER_PATTERN_INCLUDED
#include <statlog/formatter/token.hpp>

#include <array>
#include <algorithm>

namespace statlog {
    template <std::size_t N>
    class pattern_t {
    public:
        consteval pattern_t(const char(&input)[N]) {
            std::copy_n(input, N, _str.begin());
            parse();
        }

        constexpr auto begin() const {
            return _tokens.begin();
        }

        constexpr auto end() const {
            return _tokens.begin() + _ntokens;
        }

        constexpr const char* str() const {
            return _str.data();
        }

        consteval std::size_t reserve_size() const {
            std::size_t size = 0;
            for (const token& token : *this) {
                size += token.reserve_size();
            }
            return size;
        }

    private:
        consteval void parse() {
            // ignore null terminator
            for (std::size_t cursor = 0; cursor < N - 1;) { 
                if (lookahead(cursor, "%t")) {
                    add_thread_id(cursor);
                    cursor = take(cursor, "%t");
                }
                else if (lookahead(cursor, "%p")) {
                    add_process_id(cursor);
                    cursor = take(cursor, "%p");
                }
                else if (lookahead(cursor, "%n")) {
                    add_logger_name(cursor);
                    cursor = take(cursor, "%n");
                }
                else if (lookahead(cursor, "%l")) {
                    add_level_lower(cursor);
                    cursor = take(cursor, "%l");
                }
                else if (lookahead(cursor, "%L")) {
                    add_level_upper(cursor);
                    cursor = take(cursor, "%L");
                }
                // literal
                else {
                    std::size_t start = cursor;
                    while (cursor < N - 1 && _str[cursor] != '%') {
                        ++cursor;
                    }
                    add_literal(start, cursor);
                }
            }
        }

        template <std::size_t M>
        consteval bool lookahead(std::size_t from, const char(&target)[M]) const {
            for (std::size_t i = 0; i < M - 1; ++i) {   // ignore null terminator
                if (from + i >= N || _str[from + i] != target[i]) {
                    return false;
                }
            }
            return true;
        }

        template <std::size_t M>
        static consteval std::size_t take(std::size_t from, const char(&target)[M]) {
            return from + M - 1;
        }

        consteval void add_literal(size_t start, size_t end) {
            _tokens[_ntokens++] = { token_type::literal, start, end };
        }

        consteval void add_thread_id(size_t pos) {
            _tokens[_ntokens++] = { token_type::thread_id };
        }

        consteval void add_process_id(size_t pos) {
            _tokens[_ntokens++] = { token_type::process_id };
        }

        consteval void add_logger_name(size_t pos) {
            _tokens[_ntokens++] = { token_type::logger_name };
        }

        consteval void add_level_lower(size_t pos) {
            _tokens[_ntokens++] = { token_type::level_lower };
        }

        consteval void add_level_upper(size_t pos) {
            _tokens[_ntokens++] = { token_type::level_upper };
        }
    public:
        // Due to the C++20 standard, the following member variables must be public.
        std::array<char, N> _str{};
        std::array<token, N> _tokens{};
        std::size_t _ntokens = 0;
    };
    template <size_t N>
    using pattern = pattern_t<N>;
}
#endif