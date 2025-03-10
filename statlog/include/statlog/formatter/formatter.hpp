#pragma once

#ifndef STATLOG_FORMATTER_FORMATTER_INCLUDED
#define STATLOG_FORMATTER_FORMATTER_INCLUDED

#include <statlog/logger/logger.hpp>
#include <statlog/formatter/token.hpp>
#include <statlog/formatter/pattern.hpp>

#include <string>
#include <sstream>

namespace statlog {
    constexpr std::size_t SMALL_BUFFER_SIZE = 256;

    template <pattern P>
    class formatter_t {
    public:
        static constexpr std::string format(const logger_message& msg) {
            std::string result;
            result.reserve(SMALL_BUFFER_SIZE);

            for (const token& token : P) {
                switch (token.type) {
                case token_type::literal:
                    result.append(P.cstr() + token.start, token.end - token.start);
                    break;
                case token_type::message:
                    result.append(msg.message);
                    break;
                case token_type::thread_id:
                    result.append(std::format("{}", msg.thread_id));
                    break;
                case token_type::logger_name:
                    result.append(msg.logger_name);
                    break;
                case token_type::level_lower:
                    result.append(to_string_lower(msg.level));
                    break;
                case token_type::level_upper:
                    result.append(to_string_upper(msg.level));
                    break;
                case token_type::dummy:
                    std::unreachable();
                    break;
                default:
                    break;
                }
            }

            result += '\n';
            return result;
        }

    };
    template <pattern P>
    using formatter = formatter_t<P>;
}
#endif
