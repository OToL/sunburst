#include "config.h"

#include <sb_std/utility>

#if sbIsEnabled(ERROR_FACILITY)

namespace sb {
template <typename... TArgs>
usize stringFormat(char * dest_buffer, usize capacity, char const * const format, TArgs &&... args);
}

namespace sb::detail {

void reportNotImplemented(ErrorType type, char const * const file, ui32 const line,
                          char const * msg);

void reportError(ErrorType type, char const * const file, ui32 const line, char const * msg);

void reportError(ErrorType type, char const * const file, ui32 const line);

template <typename... TArgs>
void reportError(ErrorType type, char const * const file, ui32 const line, char const * msg,
                 TArgs... args)
{
    if constexpr (sizeof...(args))
    {
        char fmt_msg[255];
        sb::stringFormat(fmt_msg, sizeof(fmt_msg), msg, sbstd::forward<TArgs>(args)...);

        reportError(type, file, line, fmt_msg);
    }
}

} // namespace sb::detail

#    define sbAssertInternal(cond, ...)                                                                \
        if (!(cond))                                                                               \
        {                                                                                          \
            sb::detail::reportError(sb::ErrorType::CRITICAL, __FILE__, __LINE__, ##__VA_ARGS__);   \
        }

#    define sbWarnInternal(cond, ...)                                                                  \
        if (!(cond))                                                                               \
        {                                                                                          \
            sb::detail::reportError(sb::ErrorType::WARNING, __FILE__, __LINE__, ##__VA_ARGS__);    \
        }

#    define sbExpectInternal(cond, ...)                                                            \
        ((cond) ||                                                                                 \
         (sb::detail::reportError(sb::ErrorType::NOTICE, __FILE__, __LINE__, ##__VA_ARGS__),       \
          false))

#    define sbExpectFalseInternal(cond, ...)                                                           \
        ((cond) &&                                                                                 \
         (sb::detail::reportError(sb::ErrorType::NOTICE, __FILE__, __LINE__, ##__VA_ARGS__),       \
          true))

#    define sbNotImplementedInternal(str)                                                              \
        sb::detail::reportNotImplemented(sb::ErrorType::WARNING, __FILE__, __LINE__, str)

#else

#    define sbAssertInternal(cond, ...)
#    define sbWarnInternal(cond, ...)
#    define sbExpectInternal(cond, ...) (cond)
#    define sbExpectFalseInternal(cond, ...) !(cond)
#    define sbNotImplementedInternal(str)

#endif
