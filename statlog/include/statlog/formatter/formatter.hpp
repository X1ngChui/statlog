#pragma once

#ifndef STATLOG_FORMATTER_FORMATTER_INCLUDED
#define STATLOG_FORMATTER_FORMATTER_INCLUDED

#include <statlog/logger/logger.hpp>
#include <statlog/formatter/token.hpp>
#include <statlog/formatter/pattern.hpp>

#include <string>
#include <sstream>

namespace statlog {
    template <pattern P>
    class formatter_t {
    public:
        static std::string format(const logger_message& msg) {
            std::stringstream ss;

            for (const token& token : P) {
                switch (token.type) {
                case token_type::literal:
                    ss.write(P.cstr() + token.start, token.end - token.start);
                    break;
                case token_type::message:
                    ss << msg.message;
                    break;
                case token_type::thread_id:
                    ss << msg.thread_id;
                    break;
                case token_type::logger_name:
                    ss << msg.logger_name;
                    break;
                case token_type::level_lower:
                    ss << to_string_lower(msg.level);
                    break;
                case token_type::level_upper:
                    ss << to_string_upper(msg.level);
                    break;
                case token_type::dummy:
                    std::unreachable();
                    break;
                default:
                    break;
                }
            }

            return ss.str();
        }
    };
    template <pattern P>
    using formatter = formatter_t<P>;
}
#endif
