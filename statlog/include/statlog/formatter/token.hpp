#pragma once

#ifndef STATLOG_FORMATTER_TOKEN_INCLUDED
#define STATLOG_FORMATTER_TOKEN_INCLUDED

#include <cassert>

namespace statlog {
    enum class token_type_t { 
        literal,
        message,            // %v
        thread_id,          // %t
        logger_name,        // %n
        level_lower,        // %l
        level_upper,        // %L
        level_color_start,  // %^
        level_color_end,    // %$
        percent_sign,       // %%
        dummy
    };
    using token_type = token_type_t;

    namespace token_str {
        constexpr const char token_prefix[] = "%";
        constexpr const char message[] = "%v";
        constexpr const char thread_id[] = "%t";
        constexpr const char logger_name[] = "%n";
        constexpr const char level_lower[] = "%l";
        constexpr const char level_upper[] = "%L";
        constexpr const char level_color_start[] = "%^";
        constexpr const char level_color_end[] = "%$";
        constexpr const char percent_sign[] = "%%";
    }

    struct token_t {
        token_type type = token_type::dummy;
        
        // literal
        std::size_t start = 0;
        std::size_t end = 0;
    };
    using token = token_t;
}

#endif