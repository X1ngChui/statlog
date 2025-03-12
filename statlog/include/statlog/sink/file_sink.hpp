#pragma once

#ifndef STATLOG_SINK_FILE_SINK_INCLUDED
#define STATLOG_SINK_FILE_SINK_INCLUDED

#include <statlog/sink/sink.hpp>
#include <statlog/platform/file.hpp>
#include <statlog/utility/buffer.hpp>

namespace statlog {
    template <typename M, pattern P>
    class basic_file_sink_t : public sink<basic_file_sink_t<M, P>, M, P> {
    public:
        basic_file_sink_t(const std::filesystem::path& path, std::ios_base::openmode mode = std::ios::app, std::size_t buffer_size = 32 * 1024)
            : _file(path, mode), _buffer(buffer_size) {
        }

        basic_file_sink_t(const basic_file_sink_t&) = delete;
        basic_file_sink_t& operator=(const basic_file_sink_t&) = delete;
        basic_file_sink_t(basic_file_sink_t&&) = default;
        basic_file_sink_t& operator=(basic_file_sink_t&&) = default;

        ~basic_file_sink_t() {
            _flush();   
        }

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
                _file.write(_buffer.data(), _buffer.size());
                _file.flush();
                _buffer.clear();
            }
        }
    private:
        file _file;
        buffer<char> _buffer;
    };
    
    template <typename M, pattern P>
    using basic_file_sink = basic_file_sink_t<M, P>;

    template <pattern P>
    using file_sink_st = basic_file_sink<null_mutex, P>;
    template <pattern P>
    using file_sink_mt = basic_file_sink<mutex, P>;
}
#endif