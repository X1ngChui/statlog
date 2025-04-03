#pragma once

#ifndef STATLOG_SINK_STDOUT_SINK_INCLUDED
#define STATLOG_SINK_STDOUT_SINK_INCLUDED

#include <statlog/sink/sink.hpp>
#include <statlog/platform/stdout.hpp>
#include <array>
#include <mutex>

namespace statlog {
    template <typename M>
    class basic_stdout_sink_t : public sink<basic_stdout_sink_t<M>, M> {
    public:
        basic_stdout_sink_t() = default;
        basic_stdout_sink_t(const basic_stdout_sink_t&) = delete;
        basic_stdout_sink_t& operator=(const basic_stdout_sink_t&) = delete;
        basic_stdout_sink_t(basic_stdout_sink_t&&) = default;
        basic_stdout_sink_t& operator=(basic_stdout_sink_t&&) = default;
        ~basic_stdout_sink_t() { _flush(); }

        void _sink(const std::string& message) {
            _stdout.write(message.data(), message.size());
        }

        void _flush() {
            _stdout.flush();
        }

    private:
        stdout_t _stdout;
    };

    class stdout_mutex_t {
    public:
        void lock() { mutex_.lock(); }
        void unlock() { mutex_.unlock(); }
    private:
        inline static std::mutex mutex_{};
    };
    using stdout_mutex = stdout_mutex_t;

    using stdout_sink_st = basic_stdout_sink_t<null_mutex>;
    using stdout_sink_mt = basic_stdout_sink_t<stdout_mutex>;
}

#endif
