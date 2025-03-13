#pragma once

#ifndef STATLOG_SINK_FILE_SINK_INCLUDED
#define STATLOG_SINK_FILE_SINK_INCLUDED

#include <statlog/sink/sink.hpp>
#include <statlog/platform/file.hpp>
#include <statlog/utility/storage_units.hpp>
#include <vector>
#include <algorithm>
#include <memory>

namespace statlog {
    template <typename M, bool Sync>
    class basic_file_sink_t : public sink<basic_file_sink_t<M, Sync>, M> {
    public:
        basic_file_sink_t(const std::filesystem::path& path,
            std::ios_base::openmode mode = std::ios::app,
            std::size_t max_buffer_size = 8_MB)
            : _file(path, mode), _max_buffer_size(max_buffer_size) 
        {    
        }

        basic_file_sink_t(const basic_file_sink_t&) = delete;
        basic_file_sink_t& operator=(const basic_file_sink_t&) = delete;

        basic_file_sink_t(basic_file_sink_t&& other) noexcept = default;
        basic_file_sink_t& operator=(basic_file_sink_t&& other) noexcept = default;

        ~basic_file_sink_t() {
            _flush();
        }

        void _sink(const std::string& message) {
            _buffer.insert(_buffer.end(), message.begin(), message.end());
            if (_buffer.size() >= _max_buffer_size) {
                _flush();
            }
        }

        void _flush() {
            if (!_buffer.empty()) {
                _file.write(_buffer.data(), _buffer.size());
                if constexpr (Sync) {
                    _file.flush();
                }
                _buffer.clear();
            }
        }

    private:
        file _file;
        std::size_t _max_buffer_size;
        std::vector<char> _buffer;
    };

    template <typename M, bool Sync>
    using basic_file_sink = basic_file_sink_t<M, Sync>;

    using file_sink_st = basic_file_sink<null_mutex, false>;
    using file_sink_mt = basic_file_sink<mutex, false>;
    using sync_file_sink_st = basic_file_sink<null_mutex, true>;
    using sync_file_sink_mt = basic_file_sink<mutex, true>;
}
#endif