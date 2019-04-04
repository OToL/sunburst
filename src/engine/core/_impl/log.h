#pragma once

#include "build_target_cfg.h"
#include <core/platform.h>

#include <libc++/utility>

#if sbCTFIsEnabled(LOG_FACILITY)

namespace sb::detail {

static inline sb::LogLevel gs_log_min_level = sb::LogLevel::DEBUG;
static inline sb::b8 gs_log_quiet = false;

template <typename... TArgs>
usize stringFormat(char * dest_buffer, usize capacity, char const * const format, TArgs &&... args);

inline void setLogQuiet(b8 quiet)
{
    gs_log_quiet = quiet;
}

inline void setLogMinLevel(LogLevel min_level)
{
    gs_log_min_level = min_level;
}

void logMessage(LogLevel lvl, char const * file, ui32 line, char const * msg);

template <bool IS_ENABLED>
struct LogFilter
{
    template <typename... TArgs>
    static void logMessage(LogLevel lvl, char const * file, ui32 line, char const * msg, TArgs &&... args)
    {
        if constexpr (0 == sizeof...(args))
        {
            detail::logMessage(lvl, file, line, msg);
        }
        else
        {
            char fmt_msg[255];
            stringFormat(fmt_msg, sizeof(fmt_msg), msg, wstd::forward<TArgs>(args)...);

            detail::logMessage(lvl, file, line, fmt_msg);
        }
    }
};

template <>
struct LogFilter<false>
{
    static void logMessage(...) {}
};

} // namespace sb

#    define sbLogImpl(lvl, file, line, msg, ...) \
        (!::sb::detail::gs_log_quiet && ((::sb::ui8)lvl) <= ((::sb::ui8)sb::detail::gs_log_min_level)) && (::sb::detail::LogFilter<sbCTFIsEnabled(LOG_FACILITY)>::logMessage(lvl, file, line, msg, ##__VA_ARGS__), true)
        
#else

#    define sbLogImpl(type, file, line, msg, ...)`

namespace sb::detail {
    inline void setLogQuiet(b8) {}

    void setLogMinLevel(LogLevel) {}
}

#endif
