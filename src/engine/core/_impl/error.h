#include "build_target_cfg.h"
#include <libc++/utility>

#if sbCTFIsEnabled(ERROR_FACILITY)

namespace sb::detail {

void reportNotImplemented(ErrorType type, char const * const file, ui32 const line, char const * msg);

void reportError(ErrorType type, char const * const file, ui32 const line, char const * msg);

void reportError(ErrorType type, char const * const file, ui32 const line);

template <typename ... TArgs>
void reportError(ErrorType type, char const * const file, ui32 const line, char const * msg, TArgs ... args)
{
    if constexpr (sizeof...(args))
    {
        char fmt_msg[255];
        stringFormat(fmt_msg, msg, wstd::forward<TArgs>(args)...);

        reportError(type, file, line, fmt_msg);
    }
}

} // namespace sb::detail

#    define sbAssertImpl(cond, ...)                                                              \
        if (!(cond))                                           \
        {                                                                                        \
            sb::detail::reportError(sb::ErrorType::CRITICAL, __FILE__, __LINE__, ##__VA_ARGS__); \
        }

#    define sbWarnImpl(cond, ...)                                                               \
        if (!(cond))                                          \
        {                                                                                       \
            sb::detail::reportError(sb::ErrorType::WARNING, __FILE__, __LINE__, ##__VA_ARGS__); \
        }

#    define sbExpectTrueImpl(cond, ...) \
        ((cond) || (sb::detail::reportError(sb::ErrorType::NOTICE, __FILE__, __LINE__, ##__VA_ARGS__), false))

#    define sbExpectFalseImpl(cond, ...) \
        ((cond) && (sb::detail::reportError(sb::ErrorType::NOTICE, __FILE__, __LINE__, ##__VA_ARGS__), true))

#    define sbNotImplementedImpl(str) sb::detail::reportNotImplemented(sb::ErrorType::WARNING, __FILE__, __LINE__, str)

#else

#    define sbAssertImpl(cond, ...)
#    define sbWarnImpl(cond, ...)
#    define sbExpectTrueImpl(cond, ...) (cond)
#    define sbExpectFalseImpl(cond, ...) !(cond)
#    define sbNotImplementedImpl(str)

#endif
