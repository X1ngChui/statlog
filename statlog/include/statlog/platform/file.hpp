#pragma once

#ifndef STATLOG_PLATFORM_FILE_INCLUDED
#define STATLOG_PLATFORM_FILE_INCLUDED

#include <filesystem>
#include <fstream>

namespace statlog {
    class file_t {
    public:
        file_t(const std::filesystem::path& path, std::ios_base::openmode mode = std::ios::app)
            : _file(path, mode)
        {
        }

        ~file_t() = default;
        file_t(const file_t&) = delete;
        file_t& operator=(const file_t&) = delete;
        file_t(file_t&& other) noexcept = default;
        file_t& operator=(file_t&& other) noexcept = default;

        void write(const char* data, std::size_t size) {
            _file.write(data, size);
        }
        void flush() {
            _file.flush();
        }
    private:
        std::ofstream _file;
    };
    using file = file_t;
}

#endif