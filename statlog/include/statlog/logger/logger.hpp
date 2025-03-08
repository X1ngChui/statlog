#pragma once

#ifndef STATLOG_LOGGER_LOGGER_INCLUDED
#define STATLOG_LOGGER_LOGGER_INCLUDED

#include <statlog/logger/level.hpp>

#include <format>
#include <utility>

namespace statlog {
    template <typename L>
    class logger {
    public:
        template <typename T>
        void trace(T&& message) {
            static_cast<L*>(this)->log(level::trace, "{}", std::forward<T>(message));
        }

        template <typename... Args>
        void trace(std::format_string<Args...> fmt, Args&&... args) {
            static_cast<L*>(this)->log(level::trace, fmt, std::forward<Args>(args)...);
        }

        template <typename T>
        void debug(T&& message) {
            static_cast<L*>(this)->log(level::debug, "{}", std::forward<T>(message));
        }

        template <typename... Args>
        void debug(std::format_string<Args...> fmt, Args&&... args) {
            static_cast<L*>(this)->log(level::debug, fmt, std::forward<Args>(args)...);
        }

        template <typename T>
        void info(T&& message) {
            static_cast<L*>(this)->log(level::info, "{}", std::forward<T>(message));
        }

        template <typename... Args>
        void info(std::format_string<Args...> fmt, Args&&... args) {
            static_cast<L*>(this)->log(level::info, fmt, std::forward<Args>(args)...);
        }

        template <typename T>
        void warn(T&& message) {
            static_cast<L*>(this)->log(level::warn, "{}", std::forward<T>(message));
        }

        template <typename... Args>
        void warn(std::format_string<Args...> fmt, Args&&... args) {
            static_cast<L*>(this)->log(level::warn, fmt, std::forward<Args>(args)...);
        }

        template <typename T>
        void error(T&& message) {
            static_cast<L*>(this)->log(level::error, "{}", std::forward<T>(message));
        }

        template <typename... Args>
        void error(std::format_string<Args...> fmt, Args&&... args) {
            static_cast<L*>(this)->log(level::error, fmt, std::forward<Args>(args)...);
        }

        template <typename T>
        void fatal(T&& message) {
            static_cast<L*>(this)->log(level::fatal, "{}", std::forward<T>(message));
        }

        template <typename... Args>
        void fatal(std::format_string<Args...> fmt, Args&&... args) {
            static_cast<L*>(this)->log(level::fatal, fmt, std::forward<Args>(args)...);
        }
    };
}
#endif