#pragma once

#ifndef STATLOG_PLATFORM_OS_INCLUDED
#define STATLOG_PLATFORM_OS_INCLUDED

#include <cstddef>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace statlog {
    inline static std::size_t get_process_id() {
#ifdef _WIN32
        static std::size_t pid = static_cast<std::size_t>(GetCurrentProcessId());
        return pid;
#else
        static std::size_t pid = static_cast<std::size_t>(getpid());
        return pid;
#endif
    }
}

#endif