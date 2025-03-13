#pragma once

#ifndef STATLOG_UTILITY_STORAGE_UNITS_INCLUDED
#define STATLOG_UTILITY_STORAGE_UNITS_INCLUDED

#include <cstddef> 
namespace statlog {
    inline constexpr std::size_t operator"" _B(unsigned long long bytes) {
        return static_cast<std::size_t>(bytes);
    }

    inline constexpr std::size_t operator"" _KB(unsigned long long bytes) {
        return static_cast<std::size_t>(bytes) * 1024;
    }

    inline constexpr std::size_t operator"" _MB(unsigned long long bytes) {
        return static_cast<std::size_t>(bytes) * 1024 * 1024;
    }

    inline constexpr std::size_t operator"" _GB(unsigned long long bytes) {
        return static_cast<std::size_t>(bytes) * 1024 * 1024 * 1024;
    }

    inline constexpr std::size_t operator"" _TB(unsigned long long bytes) {
        return static_cast<std::size_t>(bytes) * 1024 * 1024 * 1024 * 1024;
    }
}

#endif