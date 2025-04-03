#pragma once

#ifndef STATLOG_SINK_FILE_SINK_INCLUDED
#define STATLOG_SINK_FILE_SINK_INCLUDED

#include <statlog/sink/sink.hpp>
#include <statlog/platform/file.hpp>
#include <vector>
#include <algorithm>
#include <memory>

namespace statlog {
    template <typename M>
    class basic_file_sink_t : public sink<basic_file_sink_t<M>, M> {
    public:
        basic_file_sink_t(const std::filesystem::path& path,
            std::ios_base::openmode mode = std::ios::app)
            : _file(path, mode)
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
            _file.write(message.data(), message.size());
        }

        void _flush() {
            _file.flush();
        }

    private:
        file _file;
    };

    template <typename M>
    using basic_file_sink = basic_file_sink_t<M>;

    using file_sink_st = basic_file_sink<null_mutex>;
    using file_sink_mt = basic_file_sink<mutex>;
}
#endif