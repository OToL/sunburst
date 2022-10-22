#include <sb_core/core.h>
#include <sb_core/compiler.h>
#include <sb_core/string/format.h>
#include <sb_core/error.h>
#include <sb_core/log.h>
#include <sb_core/cast.h>
#include <sb_core/hook.h>

#if sb_ctf_enabled(ERROR_FACILITY)

#    include <sb_slw/cassert>
#    include <sb_slw/cstdio>

static sb::ErrorHook g_error_hdl;

static constexpr char const * ERROR_DEFAULT_MSG[] = {"Critical error detected", "Unhandled error detected",
                                                     "Non fatal error detected"};

void logDefaultErrorMsg(sb::ErrorLevel type, char const * const file, sb::u32 const line, sb::ErrorCode status_code,
                        char const * msg)
{
    char fmt_msg[255];
    sb::formatString(fmt_msg, "{} ({}, {})\n\tError: {}", msg, file, line, toString(status_code));

    switch (type)
    {
        case sb::ErrorLevel::CRITICAL:
        {
            sb_log_error(&fmt_msg[0]);

            break;
        }

        case sb::ErrorLevel::WARNING:
        case sb::ErrorLevel::NOTICE:
        {
            sb_log_warning(&fmt_msg[0]);

            break;
        }
    }
}

void sb::internal::reportError(ErrorLevel type, char const * const file, u32 const line, char const * msg)
{
    reportError(type, file, line, ErrorCode::UNKNOWN, msg);
}

void sb::internal::reportError(ErrorLevel type, char const * const file, u32 const line)
{
    reportError(type, file, line, ErrorCode::UNKNOWN);
}

void sb::internal::reportNotImplemented(ErrorLevel type, char const * const file, u32 const line, char const * msg)
{
    reportNotImplemented(type, file, line, ErrorCode::UNKNOWN, msg);
}

void sb::internal::reportNotImplemented(ErrorLevel type, char const * const file, u32 const line,
                                        [[maybe_unused]] ErrorCode stastus_code, char const * msg)
{
    char msg_fmt[255];
    sb::formatString(msg_fmt, "Not implemented: '{}'", msg);
    reportError(type, file, line, &msg_fmt[0]);
}

void sb::internal::reportError(ErrorLevel type, char const * const file, u32 const line, ErrorCode stastus_code,
                               char const * msg)
{
    if (nullptr != g_error_hdl)
    {
        g_error_hdl(type, file, line, stastus_code, msg);
    }
    else
    {
        logDefaultErrorMsg(type, file, line, stastus_code, msg);
    }

    if (ErrorLevel::CRITICAL == type)
    {
        sb_debug_break();
    }
}

void sb::internal::reportError(ErrorLevel type, char const * const file, u32 const line, ErrorCode status_code)
{
    if (nullptr != g_error_hdl)
    {
        g_error_hdl(type, file, line, status_code, "");
    }
    else
    {
        logDefaultErrorMsg(type, file, line, status_code, ERROR_DEFAULT_MSG[integral_cast<>(type)]);
    }

    if ((ErrorLevel::CRITICAL == type) || (ErrorLevel::WARNING == type))
    {
        sb_debug_break();
    }
}

void sb::setErrorHook(ErrorHook const & hdl)
{
    g_error_hdl = hdl;
}

#endif

constexpr char const * ERROR_CODE_TO_STRING[] = {"OK",
                                              "CANCELLED",
                                              "INVALID_ARGUMENT",
                                              "DEADLINE_EXCEEDED",
                                              "NOT_FOUND",
                                              "ALREADY_EXISTS",
                                              "PERMISSION_DENIED",
                                              "UNAUTHENTICATED",
                                              "RESOURCE_EXHAUSTED",
                                              "FAILED_PRECONDITION",
                                              "ABORTED",
                                              "OUT_OF_RANGE",
                                              "UNIMPLEMENTED",
                                              "INTERNAL",
                                              "UNAVAILABLE",
                                              "DATA_LOSS",
                                              "UNKNOWN"};

char const * sb::toString(ErrorCode status_code)
{
    sb_assert(integral_cast<>(status_code) < sizeof(ERROR_CODE_TO_STRING));

    return ERROR_CODE_TO_STRING[integral_cast<>(status_code)];
}

