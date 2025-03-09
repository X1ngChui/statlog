#pragma once

#ifndef STATLOG_FORMATTER_TOKEN_INCLUDED
#define STATLOG_FORMATTER_TOKEN_INCLUDED

namespace statlog {
    enum class token_type_t { 
        literal,
        thread_id,
        process_id,
        logger_name,
        level_lower,
        level_upper,
        dummy
    };
    using token_type = token_type_t;

    struct token_t {
        token_type type = token_type::dummy;
        
        // literal
        std::size_t start = 0;
        std::size_t end = 0;

        consteval std::size_t reserve_size() const {
            switch (type) {
            case token_type::literal:
                return end - start;
            case token_type::thread_id:
                return 16;
            case token_type::process_id:
                return 16;
            case token_type::logger_name:
                return 16;
            case token_type::level_lower:
                return 8;
            case token_type::level_upper:
                return 8;
            }

            // Unknow token type
            return 0;
        }
    };
    using token = token_t;
}

#endif