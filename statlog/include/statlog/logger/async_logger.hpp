#pragma once

#ifndef STATLOG_LOGGER_ASYNC_LOGGER_HPP
#define STATLOG_LOGGER_ASYNC_LOGGER_HPP

#include <statlog/logger/level.hpp>
#include <statlog/logger/logger.hpp>
#include <statlog/utility/thread_pool.hpp>

namespace statlog {
    template <pattern P, typename... Sinks>
    class async_logger_t : public logger<P, async_logger_t<P, Sinks...>> {
    public:
        explicit async_logger_t(std::string_view name, sink_list<Sinks...>&& sinks, level log_level = level::info, level flush_level = level::warn, std::size_t nworkers = 2) : logger<P, async_logger_t>(name, log_level, flush_level), _sinks(std::move(sinks)), _pool(nworkers) {}

        ~async_logger_t() = default;

        void _log(level l, std::string message) {
            if (this->should_log(l)) {
                auto pmessage = std::make_shared<std::string>(std::move(message));
                std::apply([&](auto&&... sinks) {
                    (log_it(std::forward<Sinks>(sinks), l, pmessage), ...);
                    }, _sinks);
            }
        }
    private:
        template <typename S, typename... Args>
        void log_it(S&& sink, level l, std::shared_ptr<std::string> msg) 
        {
            _pool.enqueue([&, l](std::shared_ptr<std::string> message) {
                sink.sink(message);
                if (this->should_flush(l)) {
                    sink.flush();
                }
                }, std::move(msg)
            );
        }
    private:
        sink_list<Sinks...> _sinks;
        thread_pool _pool;
    };

    template <pattern P, typename... Sinks>
    auto make_async_logger(
        std::string_view name,
        sink_list<Sinks...>&& sinks,
        level log_level = level::info,
        level flush_level = level::warn,
        std::size_t nworkers = 2)
    {
        return async_logger_t<P, Sinks...>(
            name,
            std::move(sinks),
            log_level,
            flush_level,
            nworkers
        );
    }

    template <pattern P, typename... Sinks>
    using async_logger = async_logger_t<P, Sinks...>;
}
#endif