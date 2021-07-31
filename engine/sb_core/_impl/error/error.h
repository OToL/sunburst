#include <sb_core/_impl/config.h>
#include <sb_core/error/error_code.h>

#include <sb_std/utility>

#if sb_ctf_enabled(ERROR_FACILITY)

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

void reportNotImplemented(ErrorLevel type, char const * const file, u32 const line, ErrorCode stastus_code,
                          char const * msg);

void reportError(ErrorLevel type, char const * const file, u32 const line, ErrorCode stastus_code, char const * msg);

void reportError(ErrorLevel type, char const * const file, u32 const line, ErrorCode stastus_code);

template <typename... TArgs>
void reportError(ErrorLevel type, char const * const file, u32 const line, ErrorCode stastus_code, char const * msg,
                 TArgs... args)
{
    char fmt_msg[255];
    sb::stringFormat(fmt_msg, msg, sbstd::forward<TArgs>(args)...);

    reportError(type, file, line, stastus_code, &fmt_msg[0]);
}

} // namespace sb::internal

#    define sb_assert_internal(cond, ...)                                                                                \
        if (!(cond))                                                                                                   \
        {                                                                                                              \
            sb::internal::reportError(sb::ErrorLevel::CRITICAL, __FILE__, __LINE__, ##__VA_ARGS__);                    \
        }

#    define sb_warning_internal(cond, ...)                                                                                  \
        if (!(cond))                                                                                                   \
        {                                                                                                              \
            sb::internal::reportError(sb::ErrorLevel::WARNING, __FILE__, __LINE__, ##__VA_ARGS__);                     \
        }

#    define sb_expected_internal(cond, ...)                                                                                \
        ((cond) || (sb::internal::reportError(sb::ErrorLevel::NOTICE, __FILE__, __LINE__, ##__VA_ARGS__), false))

#    define sb_not_expected_internal(cond, ...)                                                                            \
        ((cond) && (sb::internal::reportError(sb::ErrorLevel::NOTICE, __FILE__, __LINE__, ##__VA_ARGS__), true))

#    define sb_not_implemented_internal(str)                                                                              \
        sb::internal::reportNotImplemented(sb::ErrorLevel::WARNING, __FILE__, __LINE__, sb::ErrorCode::UNIMPLEMENTED, str)

#else

#    define sb_assert_internal(cond, ...)
#    define sb_warning_internal(cond, ...)
#    define sb_expected_internal(cond, ...) (cond)
#    define sb_not_expected_internal(cond, ...) !(cond)
#    define sb_not_implemented_internal(str)

#endif
