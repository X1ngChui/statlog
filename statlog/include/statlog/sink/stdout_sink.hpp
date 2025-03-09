#pragma once

#ifndef STATLOG_SINK_STDOUT_SINK_INCLUDED
#define STATLOG_SINK_STDOUT_SINK_INCLUDED

#include <statlog/sink/sink.hpp>

namespace statlog {
    template <typename M, pattern P>
    class basic_stdout_sink_t : public sink<basic_stdout_sink_t<M, P>, M, P> {
    public:
        basic_stdout_sink_t(level l = level::info) : sink<basic_stdout_sink_t, M, P>(l) {}

        void _sink(const std::string& message) {
            std::fwrite(message.data(), sizeof(char), message.size(), stdout);
        }

        void _flush() {
            std::fflush(stdout);
        }
    };
    template <typename M, pattern P>
    using basic_stdout_sink = basic_stdout_sink_t<M, P>;

    class stdout_mutex_t {
    public:
        void lock() { _mutex.lock(); }
        void unlock() { _mutex.unlock(); }
    private:
        inline static std::mutex _mutex;
    };
    using stdout_mutex = stdout_mutex_t;

    template <pattern P>
    using stdout_sink_st = basic_stdout_sink<null_mutex, P>;

    template <pattern P>
    using stdout_sink_mt = basic_stdout_sink<stdout_mutex, P>;
}
#endif