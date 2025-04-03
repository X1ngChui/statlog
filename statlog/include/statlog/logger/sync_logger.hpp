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
    template <pattern P, typename... Sinks>
    class sync_logger_t : public logger<P, sync_logger_t<P, Sinks...>> {
    public:
        explicit sync_logger_t(std::string_view name, sink_list<Sinks...>&& sinks, level log_level = level::info, level flush_level = level::warn) : logger<P, sync_logger_t>(name, log_level, flush_level), _sinks(std::move(sinks)) {}

        void _log(level l, const std::string& message) {
            if (this->should_log(l)) {
                std::apply([&](auto&&... sinks) {
                    (log_it(std::forward<Sinks>(sinks), l, message), ...);
                    }, _sinks);
            }
        }
    private:
        template <typename S, typename... Args>
        void log_it(S&& sink, level l, const std::string& message)
        {
            sink.sink(message);
            if (this->should_flush(l)) {
                sink.flush();
            }
        }

    private:
        sink_list<Sinks...> _sinks;
    };

    template <pattern P, typename... Sinks>
    using sync_logger = sync_logger_t<P, Sinks...>;

    template <pattern P, typename... Sinks>
    auto make_sync_logger(
        std::string_view name,
        sink_list<Sinks...>&& sinks,
        level log_level = level::info,
        level flush_level = level::warn)
    {
        return sync_logger<P, Sinks...>(
            name,
            std::move(sinks),
            log_level,
            flush_level
        );
    }
}
#endif
