#pragma once

#ifndef STATLOG_PLATFORM_STDOUT_INCLUDED
#define STATLOG_PLATFORM_STDOUT_INCLUDED

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace statlog {

    template <bool Colorful>
    class stdout_t {
    public:
        explicit stdout_t() {
#ifdef _WIN32
            _handle = GetStdHandle(STD_OUTPUT_HANDLE);
            if constexpr (Colorful) {
                DWORD mode;
                if (GetConsoleMode(_handle, &mode)) {
                    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                    SetConsoleMode(_handle, mode);
                }
            }
#else
            _fd = STDOUT_FILENO;
#endif
        }

        void write(const char* data, size_t size) {
#ifdef _WIN32
            DWORD written;
            WriteFile(_handle, data, static_cast<DWORD>(size), &written, nullptr);
#else
            ::write(_fd, data, size);
#endif
        }
    private:
#ifdef _WIN32
        HANDLE _handle = INVALID_HANDLE_VALUE;
#else
        int _fd = -1;
#endif
    };
}
#endif