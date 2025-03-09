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

    inline static const char* to_string_lower(level l) {
        switch (l) {
        case level::trace:
            return "trace";
        case level::debug:
            return "debug";
        case level::info:
            return "info";
        case level::warn:
            return "warn";
        case level::error:
            return "error";
        case level::fatal:
            return "fatal";
        default:
            return "unknown";
        }
    }

    inline static const char* to_string_upper(level l) {
        switch (l) {
        case level::trace:
            return "TRACE";
        case level::debug:
            return "DEBUG";
        case level::info:
            return "INFO";
        case level::warn:
            return "WARN";
        case level::error:
            return "ERROR";
        case level::fatal:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }
}
#endif
