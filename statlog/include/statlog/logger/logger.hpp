#pragma once

#ifndef STATLOG_LOGGER_LOGGER_INCLUDED
#define STATLOG_LOGGER_LOGGER_INCLUDED

#include <statlog/logger/level.hpp>
#include <statlog/formatter/pattern.hpp>
#include <statlog/formatter/formatter.hpp>

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

    template <pattern P, typename L>
    class logger_t {
    public:
        explicit logger_t(std::string_view name, level_t log_level, level_t flush_level) : _name(name), _log_level(log_level), _flush_level(flush_level) {}

        template <typename T>
        void trace(T&& message) {
            log(level::trace, std::format("{}", std::forward<T>(message)));
        }

        template <typename... Args>
        void trace(std::format_string<Args...> fmt, Args&&... args) {
            log(level::trace, std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename T>
        void debug(T&& message) {
            log(level::debug, std::format("{}", std::forward<T>(message)));
        }

        template <typename... Args>
        void debug(std::format_string<Args...> fmt, Args&&... args) {
            log(level::debug, std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename T>
        void info(T&& message) {
            log(level::info, std::format("{}", std::forward<T>(message)));
        }

        template <typename... Args>
        void info(std::format_string<Args...> fmt, Args&&... args) {
            log(level::info, std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename T>
        void warn(T&& message) {
            log(level::warn, std::format("{}", std::forward<T>(message)));
        }

        template <typename... Args>
        void warn(std::format_string<Args...> fmt, Args&&... args) {
            log(level::warn, std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename T>
        void error(T&& message) {
            log(level::error, std::format("{}", std::forward<T>(message)));
        }

        template <typename... Args>
        void error(std::format_string<Args...> fmt, Args&&... args) {
            log(level::error, std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename T>
        void fatal(T&& message) {
            log(level::fatal, std::format("{}", std::forward<T>(message)));
        }

        template <typename... Args>
        void fatal(std::format_string<Args...> fmt, Args&&... args) {
            log(level::fatal, std::format(fmt, std::forward<Args>(args)...));
        }

        void log(level l, const std::string& message) {
            static_cast<L*>(this)->_log(l, 
                formatter<P>::format(logger_info{
                    .level = l,
                    .thread_id = std::this_thread::get_id(),
                    .logger_name = _name,
                    .message = message,
                    .time = std::chrono::system_clock::now()
                }));
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

    template <pattern P, typename L>
    using logger = logger_t<P, L>;
}
#endif