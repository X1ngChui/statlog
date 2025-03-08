#pragma once
#ifndef STATLOG_LOGGER_LEVEL_INCLUDED
#define STATLOG_LOGGER_LEVEL_INCLUDED

namespace statlog {
    enum class level_t {
        trace,
        debug,
        info,
        warn,
        error,
        fatal
    };
    using level = level_t;
}
#endif
