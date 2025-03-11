#pragma once

#ifndef STATLOG_LOGGER_ASYNC_LOGGER_HPP
#define STATLOG_LOGGER_ASYNC_LOGGER_HPP

#include <statlog/logger/level.hpp>
#include <statlog/logger/logger.hpp>
#include <statlog/utility/thread_pool.hpp>

namespace statlog {
    template <typename... Sinks>
    class async_logger_t : public logger<async_logger_t<Sinks...>> {
    public:
        explicit async_logger_t(std::string_view name, sink_list<Sinks...>&& sinks, level level = level::info, std::size_t nworkers = 2) : logger<async_logger_t>(name, level), _sinks(std::move(sinks)), _pool(nworkers) {}

        ~async_logger_t() = default;

        template <typename... Args>
        void log(level l, std::format_string<Args...> fmt, Args&&... args) {
            std::apply([&](auto&&... sinks) {
                (log_it(sinks, l, fmt, std::forward<Args>(args)...), ...);
                }, _sinks);
        }
    private:
        template <typename S, typename... Args>
        void log_it(S&& sink, level l, std::format_string<Args...> fmt, Args&&... args) 
        {
            if (this->should_log(l)) {
                _pool.enqueue([&](const logger_message& msg) {
                    sink.sink(msg);
                    if (this->should_flush(msg.level)) {
                        sink.flush();
                    }
                }, logger_message{
                        .level = l,
                        .thread_id = std::this_thread::get_id(),
                        .logger_name = this->name(),
                        .message = std::format(fmt, std::forward<Args>(args)...)
                    }
                );
            }
        }
    private:
        sink_list<Sinks...> _sinks;
        thread_pool _pool;
    };
    template <typename... Sinks>
    using async_logger = async_logger_t<Sinks...>;
}
#endif