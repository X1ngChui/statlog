#pragma once

#ifndef STATLOG_FORMATTER_FORMATTER_INCLUDED
#define STATLOG_FORMATTER_FORMATTER_INCLUDED

#include <statlog/logger/logger.hpp>
#include <statlog/formatter/token.hpp>
#include <statlog/formatter/pattern.hpp>

#include <string>
#include <sstream>

namespace statlog {
    constexpr std::size_t SMALL_MESSAGE_SIZE = 256;

    template <pattern P>
    class formatter_t {
    public:
        static constexpr std::string format(const logger_message& msg) {
            std::string result;
            result.reserve(SMALL_MESSAGE_SIZE);

            [&]<std::size_t... I>(std::index_sequence<I...>) {
                (format_token<P[I]>(result, msg), ...);
            }(std::make_index_sequence<P.size()>{});

            result += '\n';
            return result;
        }

    private:
        template <token T>
        static constexpr void format_token(std::string& buffer, const logger_message& msg) {
            if constexpr (T.type == token_type::literal) {
                buffer.append(P.cstr() + T.start, T.end - T.start);
            }
            else if constexpr (T.type == token_type::message) {
                buffer.append(msg.message);
            }
            else if constexpr (T.type == token_type::thread_id) {
                buffer.append(std::format("{}", msg.thread_id));
            }
            else if constexpr (T.type == token_type::logger_name) {
                buffer.append(msg.logger_name);
            }
            else if constexpr (T.type == token_type::level_lower) {
                buffer.append(to_string_lower(msg.level));
            }
            else if constexpr (T.type == token_type::level_upper) {
                buffer.append(to_string_upper(msg.level));
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
