#pragma once

#ifndef STATLOG_PLATFORM_STDOUT_INCLUDED
#define STATLOG_PLATFORM_STDOUT_INCLUDED

#include <cstdio>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace statlog {
    class stdout_t {
    public:
        explicit stdout_t() {
#ifdef _WIN32
            HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD mode;
            if (GetConsoleMode(handle, &mode)) {
                mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(handle, mode);
            }
#endif
        }
        ~stdout_t() = default;
        stdout_t(const stdout_t&) = delete;
        stdout_t& operator=(const stdout_t&) = delete;
        stdout_t(stdout_t&&) = default;
        stdout_t& operator=(stdout_t&&) = default;

        void write(const char* data, size_t size) {
            std::fwrite(data, sizeof(char), size, stdout);
        }

        void flush() {
            std::fflush(stdout);
        }
    };
}
#endif