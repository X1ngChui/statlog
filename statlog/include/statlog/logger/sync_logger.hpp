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
        explicit sync_logger_t(std::string_view name, Sinks&&... sinks) : logger<sync_logger_t>(name), _sinks(std::forward<Sinks>(sinks)...) {}

        template <typename... Args>
        void log(level l, std::format_string<Args...> fmt, Args&&... args) {  
            std::apply([&](auto&&... sink) {
                ([&]() { 
                    if (sink.should_sink(l)) {
                        sink.sink(logger_message {
                                .level = l,
                                .thread_id = std::this_thread::get_id(),
                                .logger_name = this->name(),
                                .message = std::format(fmt, std::forward<Args>(args)...)
                            });
                        if (sink.should_flush(l)) {
                            sink.flush();
                        }
                    }
                    }(), ...);
                }, _sinks);
        }

    private:
        std::tuple<Sinks...> _sinks;
    };

    template <typename... Sinks>
    using sync_logger = sync_logger_t<Sinks...>;
}
#endif
