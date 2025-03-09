#pragma once

#ifndef STATLOG_SINK_SINK_INCLUDED
#define STATLOG_SINK_SINK_INCLUDED

#include <statlog/logger/level.hpp>
#include <statlog/logger/logger.hpp>
#include <statlog/formatter/pattern.hpp>
#include <statlog/formatter/formatter.hpp>

#include <mutex>
#include <format>
#include <type_traits>
#include <concepts>
#include <memory>

namespace statlog {
    template <typename T>
    concept type_sink = requires(T t, std::string msg) {
        { t._sink(msg) } -> std::same_as<void>;
        { t._flush() } -> std::same_as<void>;
    };

    struct null_mutex_t {
        constexpr void lock() noexcept {}
        constexpr void unlock() noexcept {}
    };
    using null_mutex = null_mutex_t;

    template <typename S, typename M, pattern P>
    class sink_t {
    public:
        constexpr sink_t(level l) noexcept : _level(l) {}

        void sink(const logger_message& msg) {
            std::lock_guard<M> lock(_mutex);
            static_cast<S*>(this)->_sink(_formatter::format(msg));
        }

        void flush() {
            std::lock_guard<M> lock(_mutex);
            static_cast<S*>(this)->_flush();
        }

        void set_level(level l) {
            std::lock_guard<M> lock(_mutex);
            _level = l;
        }

        constexpr bool should_sink(level l) const noexcept {
            return l >= _level;
        }

        static constexpr bool should_flush(level l) noexcept {
            return l >= level::warn;
        }
    private:
        using _formatter = formatter<P>;
        M _mutex;
        level _level = level::info;
    };

    template <typename S, typename M, pattern P>
    using sink = sink_t<S, M, P>;

    template <typename T, typename D = std::default_delete<T>>
    using sink_pointer = std::unique_ptr<T, D>;
}
#endif