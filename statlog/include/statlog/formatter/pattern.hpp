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
            // Replace null terminator with newline
            // So the actual length of the string is N
            _str[N - 1] = '\n'; 
            parse();
        }

        consteval std::size_t size() const {
            return _ntokens;
        }

        consteval const token& operator[](std::size_t i) const {
            return _tokens[i];
        }

        consteval std::string_view literal(std::size_t begin, std::size_t end) const {
            return std::string_view(_str.data() + begin, end - begin);
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
                else if (lookahead(cursor, token_str::process_id)) {
                    cursor = add_process_id(cursor);
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
                else if (lookahead(cursor, token_str::time_HMS1)) {
                    cursor = add_timeHMS1(cursor);
                }
                else if (lookahead(cursor, token_str::time_HMS2)) {
                    cursor = add_timeHMS2(cursor);
                }
                else if (lookahead(cursor, token_str::date_MDY1)) {
                    cursor = add_dateMDY1(cursor);
                }
                else if (lookahead(cursor, token_str::date_MDY2)) {
                    cursor = add_dateMDY2(cursor);
                }
                else if (lookahead(cursor, token_str::date_and_time)) {
                    cursor = add_date_and_time(cursor);
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

        consteval bool lookahead(std::size_t from, std::string_view target) const {
            for (char c : target) {
                if (_str[from++] != c) {
                    return false;
                }
            }
            return true;
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
            next_token() = { token_type::message, cursor, cursor + token_str::message.size() };
            return cursor + token_str::message.size();
        }

        consteval std::size_t add_thread_id(std::size_t cursor) {
            next_token() = { token_type::thread_id, cursor, cursor + token_str::thread_id.size() };
            return cursor + token_str::thread_id.size();
        }

        consteval std::size_t add_process_id(std::size_t cursor) {
            next_token() = { token_type::process_id, cursor, cursor + token_str::process_id.size() };
            return cursor + token_str::process_id.size();
        }

        consteval std::size_t add_logger_name(std::size_t cursor) {
            next_token() = { token_type::logger_name, cursor, cursor + token_str::logger_name.size() };
            return cursor + token_str::logger_name.size();
        }

        consteval std::size_t add_level_lower(std::size_t cursor) {
            next_token() = { token_type::level_lower, cursor, cursor + token_str::level_lower.size() };
            return cursor + token_str::level_lower.size();
        }

        consteval std::size_t add_level_upper(std::size_t cursor) {
            next_token() = { token_type::level_upper, cursor, cursor + token_str::level_upper.size() };
            return cursor + token_str::level_upper.size();
        }

        consteval std::size_t add_level_color_start(std::size_t cursor) {
            next_token() = { token_type::level_color_start, cursor, cursor + token_str::level_color_start.size() };
            return cursor + token_str::level_color_start.size();
        }

        consteval std::size_t add_level_color_end(std::size_t cursor) {
            next_token() = { token_type::level_color_end, cursor, cursor + token_str::level_color_end.size() };
            return cursor + token_str::level_color_end.size();
        }

        consteval std::size_t add_percent_sign(std::size_t cursor) {
            next_token() = { token_type::percent_sign, cursor, cursor + token_str::percent_sign.size() };
            return cursor + token_str::percent_sign.size();
        }

        consteval std::size_t add_timeHMS1(std::size_t cursor) {
            next_token() = { token_type::time_HMS, cursor, cursor + token_str::time_HMS1.size() };
            return cursor + token_str::time_HMS1.size();
        }

        consteval std::size_t add_timeHMS2(std::size_t cursor) {
            next_token() = { token_type::time_HMS, cursor, cursor + token_str::time_HMS2.size() };
            return cursor + token_str::time_HMS2.size();
        }

        consteval std::size_t add_date_and_time(std::size_t cursor) {
            next_token() = { token_type::date_and_time, cursor, cursor + token_str::date_and_time.size() };
            return cursor + token_str::date_and_time.size();
        }

        consteval std::size_t add_dateMDY1(std::size_t cursor) {
            next_token() = { token_type::date_MDY, cursor, cursor + token_str::date_MDY1.size() };
            return cursor + token_str::date_MDY1.size();
        }

        consteval std::size_t add_dateMDY2(std::size_t cursor) {
            next_token() = { token_type::date_MDY, cursor, cursor + token_str::date_MDY2.size() };
            return cursor + token_str::date_MDY2.size();
        }
    public:
        // Due to the C++20 standard, the following member variables must be public.
        std::array<char, N> _str{};
        std::array<token, (2 * N + 1) / 3> _tokens{};
        std::size_t _ntokens = 0;
    };
    template <size_t N>
    using pattern = pattern_t<N>;
}
#endif