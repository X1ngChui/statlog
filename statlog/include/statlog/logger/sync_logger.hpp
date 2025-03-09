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
    class sync_logger : public logger<sync_logger<Sinks...>> {
    public:
        explicit sync_logger(Sinks&&... sinks) : _sinks(std::forward<Sinks>(sinks)...) {}

        template <typename... Args>
        void log(level l, std::format_string<Args...> fmt, Args&&... args) {  
            std::apply([&](auto&&... sink) {
                ([&]() { 
                    if (sink.should_sink(l)) {
                        sink.sink(std::format(fmt, std::forward<Args>(args)...));
                    }
                    if (sink.should_flush(l)) {
                        sink.flush();
                    }

                    }(), ...);
                }, _sinks);
        }

    private:
        std::tuple<Sinks...> _sinks;
    };
}
#endif
