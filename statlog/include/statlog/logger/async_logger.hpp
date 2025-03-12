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
        explicit async_logger_t(std::string_view name, sink_list<Sinks...>&& sinks, level log_level = level::info, level flush_level = level::warn, std::size_t nworkers = 2) : logger<async_logger_t>(name, log_level, flush_level), _sinks(std::move(sinks)), _pool(nworkers) {}

        ~async_logger_t() = default;

        template <typename... Args>
        void log(level l, std::format_string<Args...> fmt, Args&&... args) {
            auto pmessage = std::make_shared<logger_message>(
                logger_message{
                    .level = l,
                    .thread_id = std::this_thread::get_id(),
                    .logger_name = this->name(),
                    .message = std::format(fmt, std::forward<Args>(args)...)
                }
            );
            std::apply([&](auto&&... sinks) {
                (log_it(sinks, pmessage), ...);
                }, _sinks);
        }
    private:
        template <typename S, typename... Args>
        void log_it(S&& sink, std::shared_ptr<const logger_message> msg) 
        {
            if (this->should_log(msg->level)) {
                _pool.enqueue([&](std::shared_ptr<const logger_message> message) {
                    sink.sink(message);
                    if (this->should_flush(message->level)) {
                        sink.flush();
                    }
                }, std::move(msg)
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