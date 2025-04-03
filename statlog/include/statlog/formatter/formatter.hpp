#pragma once

#ifndef STATLOG_FORMATTER_FORMATTER_INCLUDED
#define STATLOG_FORMATTER_FORMATTER_INCLUDED

#include <statlog/logger/logger.hpp>
#include <statlog/formatter/token.hpp>
#include <statlog/formatter/pattern.hpp>
#include <statlog/platform/os.hpp>
#include <statlog/platform/time.hpp>

#include <string>
#include <format>
#include <iterator>

namespace statlog {
    template <pattern P>
    class formatter_t {
    public:
        template <typename OutputIt>
        static OutputIt format(OutputIt out, const logger_info& info) {
            [&] <std::size_t... I>(std::index_sequence<I...>) {
                (format_token<P[I]>(out, info), ...);
            }(std::make_index_sequence<P.size()>{});
            return out;
        }

    private:
        template <token T, typename OutputIt>
        static void format_token(OutputIt& out, const logger_info& info) {
            if constexpr (T.type == token_type::literal) {
                auto literal = P.literal(T.begin, T.end);
                out = std::copy(literal.begin(), literal.end(), out);
            }
            else if constexpr (T.type == token_type::message) {
                out = std::copy(info.message.begin(), info.message.end(), out);
            }
            else if constexpr (T.type == token_type::thread_id) {
                out = std::format_to(out, "{}", info.thread_id);
            }
            else if constexpr (T.type == token_type::process_id) {
                out = std::format_to(out, "{}", get_process_id());
            }
            else if constexpr (T.type == token_type::logger_name) {
                out = std::copy(info.logger_name.begin(), info.logger_name.end(), out);
            }
            else if constexpr (T.type == token_type::level_lower) {
                auto str = level_to_string_lower(info.level);
                out = std::copy(str.begin(), str.end(), out);
            }
            else if constexpr (T.type == token_type::level_upper) {
                auto str = level_to_string_upper(info.level);
                out = std::copy(str.begin(), str.end(), out);
            }
            else if constexpr (T.type == token_type::level_color_start) {
                auto color = level_to_color(info.level);
                out = std::copy(color.begin(), color.end(), out);
            }
            else if constexpr (T.type == token_type::level_color_end) {
                out = std::copy(level_color::reset.begin(), level_color::reset.end(), out);
            }
            else if constexpr (T.type == token_type::percent_sign) {
                *out++ = '%';
            }
            else if constexpr (T.type == token_type::time_HMS) {
                out = std::format_to(out, "{:02d}:{:02d}:{:02d}",
                    info.local_time.tm_hour, info.local_time.tm_min, info.local_time.tm_sec);
            }
            else if constexpr (T.type == token_type::date_MDY) {
                out = std::format_to(out, "{:02d}/{:02d}/{:02d}",
                    info.local_time.tm_mon + 1, info.local_time.tm_mday, (info.local_time.tm_year + 1900) % 100);
            }
            else if constexpr (T.type == token_type::date_and_time) {
                out = std::format_to(out,
                    "{:02d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d}",
                    info.local_time.tm_mon + 1, info.local_time.tm_mday, (info.local_time.tm_year + 1900) % 100,
                    info.local_time.tm_hour, info.local_time.tm_min, info.local_time.tm_sec);
            }
            else {
                std::unreachable();
            }
        }
    };
    template <pattern P>
    using formatter = formatter_t<P>;
}
#endif