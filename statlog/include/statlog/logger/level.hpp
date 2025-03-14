#pragma once
#ifndef STATLOG_LOGGER_LEVEL_INCLUDED
#define STATLOG_LOGGER_LEVEL_INCLUDED

#include <string>
#include <string_view>
#include <thread>
#include <ctime>

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
        constexpr std::string_view reset = "\033[0m";
        constexpr std::string_view gray = "\033[90m";
        constexpr std::string_view cyan = "\033[36m";
        constexpr std::string_view green = "\033[32m";
        constexpr std::string_view yellow = "\033[33m";
        constexpr std::string_view red = "\033[31m";
        constexpr std::string_view bold_red = "\033[1;31m";
    }

    namespace level_str {
        constexpr std::string_view trace_lower = "trace";
        constexpr std::string_view debug_lower = "debug";
        constexpr std::string_view info_lower = "info";
        constexpr std::string_view warn_lower = "warn";
        constexpr std::string_view error_lower = "error";
        constexpr std::string_view fatal_lower = "fatal";
        constexpr std::string_view unknown_lower = "unknown";
        constexpr std::string_view trace_upper = "TRACE";
        constexpr std::string_view debug_upper = "DEBUG";
        constexpr std::string_view info_upper = "INFO";
        constexpr std::string_view warn_upper = "WARN";
        constexpr std::string_view error_upper = "ERROR";
        constexpr std::string_view fatal_upper = "FATAL";
        constexpr std::string_view unknown_upper = "UNKNOWN";
    }

    inline static constexpr std::string_view level_to_string_lower(level l) {
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

    inline static constexpr std::string_view level_to_string_upper(level l) {
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

    inline static constexpr std::string_view level_to_color(level l) {
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
        std::tm local_time;
        std::tm utc_time;
    };
    using logger_info = logger_info_t;
}
#endif
