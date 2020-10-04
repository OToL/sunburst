#pragma once

#include "config.h"
#include <sb_core/core.h>

#include <sb_std/utility>

#if sbIsEnabled(LOG_FACILITY)

namespace sb::internal {

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
            internal::logMessage(lvl, file, line, msg);
        }
        else
        {
            char fmt_msg[255];
            stringFormat(fmt_msg, sizeof(fmt_msg), msg, sbstd::forward<TArgs>(args)...);

            internal::logMessage(lvl, file, line, fmt_msg);
        }
    }
};

template <>
struct LogFilter<false>
{
    static void logMessage(...) { }
};

} // namespace sb::internal

#    define sbLogImpl(lvl, file, line, msg, ...)                                                                       \
        (!::sb::internal::gs_log_quiet && ((::sb::ui8)lvl) <= ((::sb::ui8)sb::internal::gs_log_min_level)) &&          \
            (::sb::internal::LogFilter<sbIsEnabled(LOG_FACILITY)>::logMessage(lvl, file, line, msg, ##__VA_ARGS__),    \
             true)

#else

#    define sbLogImpl(type, file, line, msg, ...)`

namespace sb::internal {
inline void setLogQuiet(b8) { }

void setLogMinLevel(LogLevel) { }
} // namespace sb::internal

#endif
