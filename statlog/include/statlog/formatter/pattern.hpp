#pragma once

#ifndef STATLOG_FORMATTER_PATTERN_INCLUDED
#define STATLOG_FORMATTER_PATTERN_INCLUDED
#include <statlog/formatter/token.hpp>

#include <cassert>
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

        constexpr const char* cstr() const {
            return _str.data();
        }

    private:
        consteval void parse() {
            // ignore null terminator
            for (std::size_t cursor = 0; cursor < LEN;) {
                if (lookahead(cursor, token_str::message)) {
                    add_message();
                    cursor = take(cursor, token_str::message);
                }
                else if (lookahead(cursor, token_str::thread_id)) {
                    add_thread_id();
                    cursor = take(cursor, token_str::thread_id);
                }
                else if (lookahead(cursor, token_str::logger_name)) {
                    add_logger_name();
                    cursor = take(cursor, token_str::logger_name);
                }
                else if (lookahead(cursor, token_str::level_lower)) {
                    add_level_lower();
                    cursor = take(cursor, token_str::level_lower);
                }
                else if (lookahead(cursor, token_str::level_upper)) {
                    add_level_upper();
                    cursor = take(cursor, token_str::level_upper);
                }
                else {
                    // literal should not start with '%'
                    assert(!lookahead(cursor, token_str::token_prefix)); 

                    std::size_t start = cursor;
                    while (cursor < LEN && !lookahead(cursor, token_str::token_prefix)) {
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

        consteval token& next_token() {
            return _tokens[_ntokens++];
        }

        consteval void add_literal(size_t start, size_t end) {
            next_token() = { token_type::literal, start, end };
        }

        consteval void add_message() {
            next_token() = { token_type::message };
        }

        consteval void add_thread_id() {
            next_token() = { token_type::thread_id };
        }

        consteval void add_logger_name() {
            next_token() = { token_type::logger_name };
        }

        consteval void add_level_lower() {
            next_token() = { token_type::level_lower };
        }

        consteval void add_level_upper() {
            next_token() = { token_type::level_upper };
        }
    public:
        inline static constexpr std::size_t LEN = N - 1;

        // Due to the C++20 standard, the following member variables must be public.
        std::array<char, N> _str{};
        std::array<token, 2 * N / 3> _tokens{};
        std::size_t _ntokens = 0;
    };
    template <size_t N>
    using pattern = pattern_t<N>;
}
#endif