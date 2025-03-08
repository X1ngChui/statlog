#pragma once

#ifndef STATLOG_SINK_SINK_INCLUDED
#define STATLOG_SINK_SINK_INCLUDED

#include <statlog/logger/level.hpp>

#include <mutex>
#include <format>
#include <type_traits>
#include <concepts>

namespace statlog {
    template <typename T>
    concept type_sink = requires(T t, std::string_view msg) {
        { t._sink(msg) } -> std::same_as<void>;
        { t._flush() } -> std::same_as<void>;
    };

    struct null_mutex_t {
        constexpr void lock() noexcept {}
        constexpr void unlock() noexcept {}
    };
    using null_mutex = null_mutex_t;

    template <typename S, typename M>
    class sink_t {
    public:
        constexpr sink_t(level l) noexcept : _level(l) {}
        ~sink_t() {
            flush();
        }

        void sink(level l, std::string msg) {
            if (should_sink(l)) {
                std::lock_guard<M> lock(_mutex);
                static_cast<S*>(this)->_sink(msg);
            }
        }

        void flush() {
            std::lock_guard<M> lock(_mutex);
            static_cast<S*>(this)->_flush();
        }

        void set_level(level l) {
            std::lock_guard<M> lock(_mutex);
            _level = l;
        }

        bool should_sink(level l) const {
            return l >= _level;
        }
    private:
        M _mutex;
        level _level = level::info;
    };

    template <typename S, typename M>
    using sink = sink_t<S, M>;
}
#endif