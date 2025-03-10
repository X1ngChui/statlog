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
                    cursor = add_message(cursor);
                }
                else if (lookahead(cursor, token_str::thread_id)) {
                    cursor = add_thread_id(cursor);
                }
                else if (lookahead(cursor, token_str::logger_name)) {
                    cursor = add_logger_name(cursor);
                }
                else if (lookahead(cursor, token_str::level_lower)) {
                    cursor = add_level_lower(cursor);
                }
                else if (lookahead(cursor, token_str::level_upper)) {
                    cursor = add_level_upper(cursor);
                }
                else {
                    cursor = add_literal(cursor);
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
        static consteval std::size_t token_size(const char(&target)[M]) {
            return M - 1;
        }

        consteval token& next_token() {
            return _tokens[_ntokens++];
        }

        consteval std::size_t add_literal(std::size_t cursor) {
            // literal should not start with '%'
            assert(!lookahead(cursor, token_str::token_prefix));

            std::size_t start = cursor;
            while (cursor < LEN && !lookahead(cursor, token_str::token_prefix)) {
                ++cursor;
            }

            next_token() = { token_type::literal, start, cursor };
            return cursor;
        }

        consteval std::size_t add_message(std::size_t cursor) {
            next_token() = { token_type::message, cursor, cursor + token_size(token_str::message)};
            return cursor + token_size(token_str::message);
        }

        consteval std::size_t add_thread_id(std::size_t cursor) {
            next_token() = { token_type::thread_id, cursor, cursor + token_size(token_str::thread_id)};
            return cursor + token_size(token_str::thread_id);
        }

        consteval std::size_t add_logger_name(std::size_t cursor) {
            next_token() = { token_type::logger_name, cursor, cursor + token_size(token_str::logger_name)};
            return cursor + token_size(token_str::logger_name);
        }

        consteval std::size_t add_level_lower(std::size_t cursor) {
            next_token() = { token_type::level_lower, cursor, cursor + token_size(token_str::level_lower)};
            return cursor + token_size(token_str::level_lower);
        }

        consteval std::size_t add_level_upper(std::size_t cursor) {
            next_token() = { token_type::level_upper, cursor, cursor + token_size(token_str::level_upper)};
            return cursor + token_size(token_str::level_upper);
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