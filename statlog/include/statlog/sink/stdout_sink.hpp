#pragma once

#ifndef STATLOG_SINK_STDOUT_SINK_INCLUDED
#define STATLOG_SINK_STDOUT_SINK_INCLUDED

#include <statlog/sink/sink.hpp>
#include <statlog/platform/stdout.hpp>
#include <statlog/utility/inline_vector.hpp>
#include <array>
#include <mutex>

namespace statlog {
    template <typename M, pattern P, bool Colorful>
    class basic_stdout_sink_t : public sink<basic_stdout_sink_t<M, P, Colorful>, M, P> {
    public:
        basic_stdout_sink_t() = default;
        basic_stdout_sink_t(const basic_stdout_sink_t&) = delete;
        basic_stdout_sink_t& operator=(const basic_stdout_sink_t&) = delete;
        basic_stdout_sink_t(basic_stdout_sink_t&&) = default;
        basic_stdout_sink_t& operator=(basic_stdout_sink_t&&) = default;
        ~basic_stdout_sink_t() { _flush(); }

        void _sink(const std::string& message) {
            const char* data = message.data();
            std::size_t size = message.size();

            while (size > 0) {
                if (_buffer.size() >= _buffer.capacity()) {
                    _flush();
                }

                const std::size_t available = _buffer.capacity() - _buffer.size();
                assert(available > 0);
                const std::size_t chunk = std::min(size, available);
                assert(chunk > 0);

                _buffer.insert(_buffer.end(), data, data + chunk);
                data += chunk;
                size -= chunk;
            }
        }

        void _flush() {
            if (_buffer.size() > 0) {
                _stdout.write(_buffer.data(), _buffer.size());
                _buffer.clear();
                assert(_buffer.capacity() > _buffer.size());
            }
        }

    private:
        inline static constexpr std::size_t BUFFER_SIZE = 8192;
        stdout_t<Colorful> _stdout;
        inline_vector<char, BUFFER_SIZE> _buffer{};
    };

    class stdout_mutex_t {
    public:
        void lock() { mutex_.lock(); }
        void unlock() { mutex_.unlock(); }
    private:
        inline static std::mutex mutex_{};
    };
    using stdout_mutex = stdout_mutex_t;

    template <pattern P>
    using stdout_sink_st = basic_stdout_sink_t<null_mutex, P, false>;

    template <pattern P>
    using stdout_sink_mt = basic_stdout_sink_t<stdout_mutex, P, false>;

    template <pattern P>
    using colorful_stdout_sink_st = basic_stdout_sink_t<null_mutex, P, true>;

    template <pattern P>
    using colorful_stdout_sink_mt = basic_stdout_sink_t<stdout_mutex, P, true>;
}

#endif
