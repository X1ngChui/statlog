#pragma once

#ifndef STATLOG_LOGGER_SYNC_LOGGER_INCLUDED
#define STATLOG_LOGGER_SYNC_LOGGER_INCLUDED

#include <statlog/logger/level.hpp>
#include <statlog/logger/logger.hpp>
#include <statlog/sink/sink.hpp>

#include <format>
#include <type_traits>
#include <utility>

namespace statlog {
    template <typename... Sinks>
    class sync_logger_t : public logger<sync_logger_t<Sinks...>> {
    public:
        explicit sync_logger_t(std::string_view name, sink_list<Sinks...>&& sinks, level level = level::info) : logger<sync_logger_t>(name, level), _sinks(std::move(sinks)) {}

        template <typename... Args>
        void log(level l, std::format_string<Args...> fmt, Args&&... args) {  
            logger_message msg{
                    .level = l,
                    .thread_id = std::this_thread::get_id(),
                    .logger_name = this->name(),
                    .message = std::format(fmt, std::forward<Args>(args)...)
            };

            std::apply([&](auto&&... sinks) {
                (log_it(sinks, msg), ...);
            }, _sinks);
        }
    private:
        template <typename S, typename... Args>
        void log_it(S&& sink, const logger_message& msg)
        {
            if (this->should_log(msg.level)) {
                sink.sink(msg);
                if (this->should_flush(msg.level)) {
                    sink.flush();
                }
            }
        }

    private:
        sink_list<Sinks...> _sinks;
    };

    template <typename... Sinks>
    using sync_logger = sync_logger_t<Sinks...>;
}
#endif
