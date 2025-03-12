#pragma once

#ifndef STATLOG_LOGGER_LOGGER_INCLUDED
#define STATLOG_LOGGER_LOGGER_INCLUDED

#include <statlog/logger/level.hpp>

#include <format>
#include <utility>
#include <thread>
#include <type_traits>

namespace statlog {
    template <typename... Sinks>
    using sink_list = std::tuple<Sinks...>;

    template <typename... Sinks>
    auto make_sink_list(Sinks&&... sinks) {
        return sink_list<Sinks...>(std::forward<Sinks>(sinks)...);
    }

    template <typename L>
    class logger_t {
    public:
        explicit logger_t(std::string_view name, level_t log_level, level_t flush_level) : _name(name), _log_level(log_level), _flush_level(flush_level) {}

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

        const std::string& name() const noexcept {
            return _name;
        }

        level_t level() const noexcept {
            return _log_level;
        }

        level_t flush_level() const noexcept {
            return _flush_level;
        }

        void set_level(level_t l) noexcept {
            _log_level = l;
        }

        void flush_on(level_t l) noexcept {
            _flush_level = l;
        }

    protected:
        inline bool should_log(statlog::level l) const noexcept {
            return l >= _log_level;
        }

        inline bool should_flush(statlog::level l) noexcept {
            return l >= _flush_level;
        }

    private:
        std::string _name;
        level_t _log_level = level::info;
        level_t _flush_level = level::warn;
    };

    template <typename L>
    using logger = logger_t<L>;

    struct logger_message_t {
        level level;
        std::thread::id thread_id;
        const std::string& logger_name;
        std::string message;
    };
    using logger_message = logger_message_t;
}
#endif