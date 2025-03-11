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
            _str[N - 1] = '\n'; // replace null terminator with newline
            parse();
        }

        consteval std::size_t size() const {
            return _ntokens;
        }

        consteval const token& operator[](std::size_t i) const {
            return _tokens[i];
        }

        consteval const char* cstr() const {
            return _str.data();
        }

    private:
        consteval void parse() {
            for (std::size_t cursor = 0; cursor < N;) {
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
                else if (lookahead(cursor, token_str::level_color_start)) {
                    cursor = add_level_color_start(cursor);
                }
                else if (lookahead(cursor, token_str::level_color_end)) {
                    cursor = add_level_color_end(cursor);
                }
                else if (lookahead(cursor, token_str::percent_sign)) {
                    cursor = add_percent_sign(cursor);
                }
                else {
                    cursor = add_literal(cursor);
                }
            }
        }

        consteval void remove_token(std::size_t index) {
            for (std::size_t i = index; i < size() - 1; i++) {
                _tokens[i] = _tokens[i + 1];
            }
            _tokens[--_ntokens] = { token_type::dummy };
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
            while (cursor < N && !lookahead(cursor, token_str::token_prefix)) {
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

        consteval std::size_t add_level_color_start(std::size_t cursor) {
            next_token() = { token_type::level_color_start, cursor, cursor + token_size(token_str::level_color_start) };
            return cursor + token_size(token_str::level_color_start);
        }

        consteval std::size_t add_level_color_end(std::size_t cursor) {
            next_token() = { token_type::level_color_end, cursor, cursor + token_size(token_str::level_color_end) };
            return cursor + token_size(token_str::level_color_end);
        }

        consteval std::size_t add_percent_sign(std::size_t cursor) {
            next_token() = { token_type::percent_sign, cursor, cursor + token_size(token_str::percent_sign) };
            return cursor + token_size(token_str::percent_sign);
        }
    public:
        // Due to the C++20 standard, the following member variables must be public.
        std::array<char, N> _str{};
        std::array<token, 2 * N / 3 + 1> _tokens{};
        std::size_t _ntokens = 0;
    };
    template <size_t N>
    using pattern = pattern_t<N>;
}
#endif