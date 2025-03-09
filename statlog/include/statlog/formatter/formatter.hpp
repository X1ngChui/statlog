#pragma once

#ifndef STATLOG_FORMATTER_FORMATTER_INCLUDED
#define STATLOG_FORMATTER_FORMATTER_INCLUDED

#include <statlog/formatter/token.hpp>
#include <statlog/formatter/pattern.hpp>

#include <string>

namespace statlog {
    using formatter_buffer = std::string;

    template <pattern P>
    class formatter_t {
    public:
        static void format(formatter_buffer& buffer) {
            constexpr std::size_t reserve_size = P.reserve_size();
            buffer.reserve(reserve_size);

            for (const token& token : P) {
                switch (token.type) {
                case token_type::literal:
                    buffer.append(P.str() + token.start, token.end - token.start);
                    break;
                case token_type::thread_id:
                    buffer += "thread_id";
                    break;
                case token_type::process_id:
                    buffer += "process_id";
                    break;
                case token_type::logger_name:
                    buffer += "logger_name";
                    break;
                case token_type::level_lower:
                    buffer += "level_lower";
                    break;
                case token_type::level_upper:
                    buffer += "level_upper";
                    break;
                default:
                    break;
                }
            }
        }
    };
    template <pattern P>
    using formatter = formatter_t<P>;
}
#endif
