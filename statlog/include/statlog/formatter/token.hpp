#pragma once

#ifndef STATLOG_FORMATTER_TOKEN_INCLUDED
#define STATLOG_FORMATTER_TOKEN_INCLUDED

#include <cassert>

namespace statlog {
    enum class token_type_t { 
        literal,
        message,            // %v
        thread_id,          // %t
        process_id,         // %P
        logger_name,        // %n
        level_lower,        // %l
        level_upper,        // %L
        level_color_start,  // %^
        level_color_end,    // %$
        percent_sign,       // %%
        time_HMS,           // %T %X
        date_MDY,           // %D %x
        date_and_time,      // %c
        dummy
    };
    using token_type = token_type_t;

    namespace token_str {
        constexpr std::string_view token_prefix = "%";
        constexpr std::string_view message = "%v";
        constexpr std::string_view thread_id = "%t";
        constexpr std::string_view process_id = "%P";
        constexpr std::string_view logger_name = "%n";
        constexpr std::string_view level_lower = "%l";
        constexpr std::string_view level_upper = "%L";
        constexpr std::string_view level_color_start = "%^";
        constexpr std::string_view level_color_end = "%$";
        constexpr std::string_view percent_sign = "%%";
        constexpr std::string_view time_HMS1 = "%T";
        constexpr std::string_view time_HMS2 = "%X";
        constexpr std::string_view date_MDY1 = "%D";
        constexpr std::string_view date_MDY2 = "%x";
        constexpr std::string_view date_and_time = "%c";
    }

    struct token_t {
        token_type type = token_type::dummy;
        
        // Token's begin and end positions in the pattern string
        std::size_t begin = 0;
        std::size_t end = 0;
    };
    using token = token_t;
}

#endif