#pragma once

#ifndef STATLOG_SINK_SINK_INCLUDED
#define STATLOG_SINK_SINK_INCLUDED

#include <statlog/logger/level.hpp>
#include <statlog/logger/logger.hpp>

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

    struct mutex_t {
    public:
        void lock() { _mutex->lock(); }
        void unlock() { _mutex->unlock(); }

    private:
        std::unique_ptr<std::mutex> _mutex = std::make_unique<std::mutex>();
    };
    using mutex = mutex_t;
    static_assert(std::is_move_constructible_v<mutex>);

    template <typename S, typename M>
    class sink_t {
    public:
        void sink(std::shared_ptr<std::string> msg) {
            std::lock_guard<M> lock(_mutex);
            static_cast<S*>(this)->_sink(*msg);
        }

        void sink(const std::string& msg) {
            std::lock_guard<M> lock(_mutex);
            static_cast<S*>(this)->_sink(msg);
        }

        void flush() {
            std::lock_guard<M> lock(_mutex);
            static_cast<S*>(this)->_flush();
        }
    private:
        M _mutex;
    };

    template <typename S, typename M>
    using sink = sink_t<S, M>;
}
#endif