#pragma once

#ifndef STATLOG_SINK_STDOUT_SINK_INCLUDED
#define STATLOG_SINK_STDOUT_SINK_INCLUDED

#include <statlog/sink/sink.hpp>
#include <statlog/platform/stdout.hpp>
#include <statlog/utility/storage_units.hpp>
#include <array>
#include <mutex>

namespace statlog {
    template <typename M, bool Colorful>
    class basic_stdout_sink_t : public sink<basic_stdout_sink_t<M, Colorful>, M> {
    public:
        basic_stdout_sink_t(std::size_t max_buffer_size = 8_KB) : _max_buffer_size(max_buffer_size) {}
        basic_stdout_sink_t(const basic_stdout_sink_t&) = delete;
        basic_stdout_sink_t& operator=(const basic_stdout_sink_t&) = delete;
        basic_stdout_sink_t(basic_stdout_sink_t&&) = default;
        basic_stdout_sink_t& operator=(basic_stdout_sink_t&&) = default;
        ~basic_stdout_sink_t() { _flush(); }

        void _sink(const std::string& message) {
            _buffer.insert(_buffer.end(), message.begin(), message.end());
            if (_buffer.size() >= _max_buffer_size) {
                _flush();
            }
        }

        void _flush() {
            if (_buffer.size() > 0) {
                _stdout.write(_buffer.data(), _buffer.size());
                _buffer.clear();
            }
        }

    private:
        stdout_t<Colorful> _stdout;
        std::size_t _max_buffer_size;
        std::vector<char> _buffer;
    };

    class stdout_mutex_t {
    public:
        void lock() { mutex_.lock(); }
        void unlock() { mutex_.unlock(); }
    private:
        inline static std::mutex mutex_{};
    };
    using stdout_mutex = stdout_mutex_t;

    using stdout_sink_st = basic_stdout_sink_t<null_mutex, false>;
    using stdout_sink_mt = basic_stdout_sink_t<stdout_mutex, false>;
    using colorful_stdout_sink_st = basic_stdout_sink_t<null_mutex, true>;
    using colorful_stdout_sink_mt = basic_stdout_sink_t<stdout_mutex, true>;
}

#endif
