#include <sb_core/core.h>
#include <sb_core/compiler.h>
#include <sb_core/string/string_format.h>
#include <sb_core/error/error.h>
#include <sb_core/log.h>
#include <sb_core/enum.h>
#include <sb_core/hook.h>

#if sbIsEnabled(ERROR_FACILITY)

#    include <sb_std/cassert>
#    include <sb_std/cstdio>

static sb::ErrorHandler g_error_hdl;

static constexpr char const * ERROR_DEFAULT_MSG[] = {"Critical error detected", "Unhandled error detected",
                                                     "Non fatal error detected"};

void logDefaultErrorMsg(sb::ErrorLevel type, char const * const file, sb::u32 const line, sb::StatusCode status_code,
                        char const * msg)
{
    char fmt_msg[255];
    sb::stringFormat(fmt_msg, "{} ({}, {})\n\tStatus: {}", msg, file, line, statuscode_getValueName(status_code));

    switch (type)
    {
        case sb::ErrorLevel::CRITICAL:
        {
            sbLogE(&fmt_msg[0]);

            break;
        }

        case sb::ErrorLevel::WARNING:
        case sb::ErrorLevel::NOTICE:
        {
            sbLogW(&fmt_msg[0]);

            break;
        }
    }
}

void sb::internal::reportError(ErrorLevel type, char const * const file, u32 const line, char const * msg)
{
    reportError(type, file, line, StatusCode::UNKNOWN, msg);
}

void sb::internal::reportError(ErrorLevel type, char const * const file, u32 const line)
{
    reportError(type, file, line, StatusCode::UNKNOWN);
}

void sb::internal::reportNotImplemented(ErrorLevel type, char const * const file, u32 const line, char const * msg)
{
    reportNotImplemented(type, file, line, StatusCode::UNKNOWN, msg);
}

void sb::internal::reportNotImplemented(ErrorLevel type, char const * const file, u32 const line,
                                        [[maybe_unused]] StatusCode stastus_code, char const * msg)
{
    char msg_fmt[255];
    sb::stringFormat(msg_fmt, "Not implemented: '{}'", msg);
    reportError(type, file, line, &msg_fmt[0]);
}

void sb::internal::reportError(ErrorLevel type, char const * const file, u32 const line, StatusCode stastus_code,
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
        sb::debugBreak();
    }
}

void sb::internal::reportError(ErrorLevel type, char const * const file, u32 const line, StatusCode status_code)
{
    if (nullptr != g_error_hdl)
    {
        g_error_hdl(type, file, line, status_code, "");
    }
    else
    {
        logDefaultErrorMsg(type, file, line, status_code, ERROR_DEFAULT_MSG[getEnumValue(type)]);
    }

    if ((ErrorLevel::CRITICAL == type) || (ErrorLevel::WARNING == type))
    {
        sb::debugBreak();
    }
}

void sb::setErrorHandler(ErrorHandler const & hdl)
{
    g_error_hdl = hdl;
}

#endif
