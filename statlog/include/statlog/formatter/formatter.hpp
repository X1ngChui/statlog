#pragma once

#ifndef STATLOG_FORMATTER_FORMATTER_INCLUDED
#define STATLOG_FORMATTER_FORMATTER_INCLUDED

#include <statlog/logger/logger.hpp>
#include <statlog/formatter/token.hpp>
#include <statlog/formatter/pattern.hpp>
#include <statlog/platform/os.hpp>

#include <string>
#include <format>

namespace statlog {
    constexpr std::size_t SMALL_MESSAGE_SIZE = 256;

    template <pattern P>
    class formatter_t {
    public:
        static constexpr std::string format(const logger_info& info) {
            std::string result;
            result.reserve(SMALL_MESSAGE_SIZE);
    
            [&] <std::size_t... I>(std::index_sequence<I...>) {
                (format_token<P[I]>(result, info), ...);
            }(std::make_index_sequence<P.size()>{});

            return result;
        }

    private:
        template <token T>
        static constexpr void format_token(std::string& buffer, const logger_info& info) {
            if constexpr (T.type == token_type::literal) {
                buffer.append(P.literal(T.begin, T.end));
            }
            else if constexpr (T.type == token_type::message) {
                buffer.append(info.message);
            }
            else if constexpr (T.type == token_type::thread_id) {
                std::format_to(std::back_inserter(buffer), "{}", info.thread_id);
            }
            else if constexpr (T.type == token_type::process_id) {
                std::format_to(std::back_inserter(buffer), "{}", get_process_id());
            }
            else if constexpr (T.type == token_type::logger_name) {
                buffer.append(info.logger_name);
            }
            else if constexpr (T.type == token_type::level_lower) {
                buffer.append(level_to_string_lower(info.level));
            }
            else if constexpr (T.type == token_type::level_upper) {
                buffer.append(level_to_string_upper(info.level));
            }
            else if constexpr (T.type == token_type::level_color_start) {
                buffer.append(level_to_color(info.level));
            }
            else if constexpr (T.type == token_type::level_color_end) {
                buffer.append(level_color::reset);
            }
            else if constexpr (T.type == token_type::percent_sign) {
                buffer += '%';
            }
            else if constexpr (T.type == token_type::time_HMS) {
                auto local_time = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(info.time));
                std::format_to(std::back_inserter(buffer), "{:%H:%M:%S}", local_time);
            }
            else if constexpr (T.type == token_type::date_MDY) {
                auto local_time = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(info.time));
                std::format_to(std::back_inserter(buffer), "{:%m/%d/%y}", local_time);
            }
            else if constexpr (T.type == token_type::date_and_time) {
                auto local_time = std::chrono::zoned_time(std::chrono::current_zone(), info.time);
                std::format_to(std::back_inserter(buffer), "{:%c}", local_time);
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
