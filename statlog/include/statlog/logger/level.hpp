#pragma once
#ifndef STATLOG_LOGGER_LEVEL_INCLUDED
#define STATLOG_LOGGER_LEVEL_INCLUDED

namespace statlog {
    enum class level_t {
        trace,
        debug,
        info,
        warn,
        error,
        fatal
    };
    using level = level_t;

    namespace level_color {
        constexpr const char* reset = "\033[0m";
        constexpr const char* gray = "\033[90m";
        constexpr const char* cyan = "\033[36m";
        constexpr const char* green = "\033[32m";
        constexpr const char* yellow = "\033[33m";
        constexpr const char* red = "\033[31m";
        constexpr const char* bold_red = "\033[1;31m";
    }

    namespace level_str {
        constexpr const char* trace_lower = "trace";
        constexpr const char* debug_lower = "debug";
        constexpr const char* info_lower = "info";
        constexpr const char* warn_lower = "warn";
        constexpr const char* error_lower = "error";
        constexpr const char* fatal_lower = "fatal";
        constexpr const char* unknown_lower = "unknown";
        constexpr const char* trace_upper = "TRACE";
        constexpr const char* debug_upper = "DEBUG";
        constexpr const char* info_upper = "INFO";
        constexpr const char* warn_upper = "WARN";
        constexpr const char* error_upper = "ERROR";
        constexpr const char* fatal_upper = "FATAL";
        constexpr const char* unknown_upper = "UNKNOWN";
    }

    inline static const char* level_to_string_lower(level l) {
        switch (l) {
        case level::trace:
            return level_str::trace_lower;
        case level::debug:
            return level_str::debug_lower;
        case level::info:
            return level_str::info_lower;
        case level::warn:
            return level_str::warn_lower;
        case level::error:
            return level_str::error_lower;
        case level::fatal:
            return level_str::fatal_lower;
        }
        return level_str::unknown_lower;
    }

    inline static const char* level_to_string_upper(level l) {
        switch (l) {
        case level::trace:
            return level_str::trace_upper;
        case level::debug:
            return level_str::debug_upper;
        case level::info:
            return level_str::info_upper;
        case level::warn:
            return level_str::warn_upper;
        case level::error:
            return level_str::error_upper;
        case level::fatal:
            return level_str::fatal_upper;
        }
        return level_str::unknown_upper;
    }

    inline static const char* level_to_color(level l) {
        switch (l) {
        case level::trace:
            return level_color::gray;
        case level::debug:
            return level_color::cyan;
        case level::info:
            return level_color::green;
        case level::warn:
            return level_color::yellow;
        case level::error:
            return level_color::red;
        case level::fatal:
            return level_color::bold_red;
        }
        return level_color::reset;
    }

    struct logger_info_t {
        level level;
        std::thread::id thread_id;
        const std::string& logger_name;
        const std::string& message;
    };
    using logger_info = logger_info_t;
}
#endif
