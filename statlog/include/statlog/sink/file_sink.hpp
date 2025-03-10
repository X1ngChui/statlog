#pragma once

#ifndef STATLOG_SINK_FILE_SINK_INCLUDED
#define STATLOG_SINK_FILE_SINK_INCLUDED

#include <statlog/sink/sink.hpp>

#include <filesystem>

namespace statlog {
    template <typename M, pattern P>
    class basic_file_sink_t : public sink<basic_file_sink_t<M, P>, M, P> {
    public:
        basic_file_sink_t(const std::filesystem::path& path, std::ios_base::openmode mode = std::ios::app)
            : _os(path, mode) {
        }

        basic_file_sink_t(const basic_file_sink_t&) = delete;
        basic_file_sink_t& operator=(const basic_file_sink_t&) = delete;
        basic_file_sink_t(basic_file_sink_t&&) = default;
        basic_file_sink_t& operator=(basic_file_sink_t&&) = default;

        ~basic_file_sink_t() {
            _flush();   
        }

        void _sink(const std::string& message) {
            _os << message;
        }

        void _flush() {
            _os.flush();
        }
    private:
        std::ofstream _os;
    };
    
    template <typename M, pattern P>
    using basic_file_sink = basic_file_sink_t<M, P>;

    template <pattern P>
    using file_sink_st = basic_file_sink<null_mutex, P>;
    template <pattern P>
    using file_sink_mt = basic_file_sink<mutex, P>;
}
#endif