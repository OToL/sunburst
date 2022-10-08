#pragma once

#include "config.h"
#include <sb_core/core.h>
#include <sb_core/string/format.h>

#include <sb_std/utility>

#if sb_ctf_enabled(LOG_FACILITY)

namespace sb::internal {

inline sb::LogLevel gs_log_min_level = sb::LogLevel::DEBUG;
inline sb::b8 gs_log_quiet = false;

inline void setLogQuiet(b8 quiet)
{
    gs_log_quiet = quiet;
}

inline void setLogMinLevel(LogLevel min_level)
{
    gs_log_min_level = min_level;
}

void logMessage(LogLevel lvl, char const * file, u32 line, char const * msg);

template <bool IS_ENABLED>
struct LogFilter
{
    template <typename... TArgs>
    static void logMessage(LogLevel lvl, char const * file, u32 line, char const * msg, TArgs &&... args)
    {
        if constexpr (0 == sizeof...(args))
        {
            internal::logMessage(lvl, file, line, msg);
        }
        else
        {
            char fmt_msg[255];
            formatString(fmt_msg, sizeof(fmt_msg), msg, sbstd::forward<TArgs>(args)...);

            internal::logMessage(lvl, file, line, &fmt_msg[0]);
        }
    }
};

template <>
struct LogFilter<false>
{
    static void logMessage(...) { }
};

} // namespace sb::internal

#    define sb_log_internal(lvl, file, line, msg, ...)                                                                       \
        (!::sb::internal::gs_log_quiet && ((::sb::u8)lvl) <= ((::sb::u8)sb::internal::gs_log_min_level)) &&            \
            (::sb::internal::LogFilter<sb_ctf_enabled(LOG_FACILITY)>::logMessage(lvl, file, line, msg, ##__VA_ARGS__),    \
             true)

#else

#    define sb_log_internal(type, file, line, msg, ...)`

namespace sb::internal {
inline void setLogQuiet(b8) { }

void setLogMinLevel(LogLevel) { }
} // namespace sb::internal

#endif
