#pragma once

#ifndef STATLOG_PLATFORM_TIME_INCLUDED
#define STATLOG_PLATFORM_TIME_INCLUDED

#include <ctime>

namespace statlog::time {
    inline static std::tm local_time(const std::time_t& timestamp) {
        std::tm time;
#ifdef _WIN32
        localtime_s(&time, &timestamp);
#else
        localtime_r(&timestamp, &time);
#endif 
        return time;
    }

    inline static struct std::tm utc_time(const std::time_t& timestamp) {
        std::tm time;
#ifdef _WIN32
        gmtime_s(&time, &timestamp);
#else
        gmtime_r(&timestamp, &time);
#endif
        return time;
    }
}

#endif