#include "config.h"

#include <sb_std/utility>

#if sbIsEnabled(ERROR_FACILITY)

#    include <sb_std/span>

namespace sb {
template <typename... TArgs>
usize stringFormat(sbstd::span<char> dest_buffer, char const * const format, TArgs &&... args);
}

namespace sb::internal {

void reportNotImplemented(ErrorLevel type, char const * const file, u32 const line, char const * msg);

void reportError(ErrorLevel type, char const * const file, u32 const line, char const * msg);

void reportError(ErrorLevel type, char const * const file, u32 const line);

template <typename... TArgs>
void reportError(ErrorLevel type, char const * const file, u32 const line, char const * msg, TArgs... args)
{
    char fmt_msg[255];
    sb::stringFormat(fmt_msg, msg, sbstd::forward<TArgs>(args)...);

    reportError(type, file, line, &fmt_msg[0]);
}

} // namespace sb::internal

#    define sbAssertInternal(cond, ...)                                                                                \
        if (!(cond))                                                                                                   \
        {                                                                                                              \
            sb::internal::reportError(sb::ErrorLevel::CRITICAL, __FILE__, __LINE__, ##__VA_ARGS__);                    \
        }

#    define sbWarnInternal(cond, ...)                                                                                  \
        if (!(cond))                                                                                                   \
        {                                                                                                              \
            sb::internal::reportError(sb::ErrorLevel::WARNING, __FILE__, __LINE__, ##__VA_ARGS__);                     \
        }

#    define sbExpectInternal(cond, ...)                                                                                \
        ((cond) || (sb::internal::reportError(sb::ErrorLevel::NOTICE, __FILE__, __LINE__, ##__VA_ARGS__), false))

#    define sbDontExpectInternal(cond, ...)                                                                            \
        ((cond) && (sb::internal::reportError(sb::ErrorLevel::NOTICE, __FILE__, __LINE__, ##__VA_ARGS__), true))

#    define sbNotImplementedInternal(str)                                                                              \
        sb::internal::reportNotImplemented(sb::ErrorLevel::WARNING, __FILE__, __LINE__, str)

#else

#    define sbAssertInternal(cond, ...)
#    define sbWarnInternal(cond, ...)
#    define sbExpectInternal(cond, ...) (cond)
#    define sbDontExpectInternal(cond, ...) !(cond)
#    define sbNotImplementedInternal(str)

#endif
