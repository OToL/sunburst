#include <sb_core/core.h>
#include <sb_core/compiler.h>
#include <sb_core/string/string_format.h>
#include <sb_core/error.h>
#include <sb_core/log.h>
#include <sb_core/enum.h>

#if sbIsEnabled(ERROR_FACILITY)

#    include <sb_std/cassert>
#    include <sb_std/cstdio>

static sb::ErrorHandler g_error_hdl;

static constexpr char const * ERROR_DEFAULT_MSG[] = {"Critical error detected", "Unhandled error detected",
                                                     "Non fatal error detected"};

void logDefaultErrorMsg(sb::ErrorLevel type, char const * const file, sb::u32 const line, char const * msg)
{
    char fmt_msg[255];
    sb::stringFormat(fmt_msg, "{} ({}, {})", msg, file, line);

    switch (type)
    {
        case sb::ErrorLevel::CRITICAL:
        {
            sbLogE(fmt_msg);

            break;
        }

        case sb::ErrorLevel::WARNING:
        case sb::ErrorLevel::NOTICE:
        {
            sbLogW(fmt_msg);

            break;
        }
    }
}

void sb::internal::reportError(ErrorLevel type, char const * const file, u32 const line, char const * msg)
{
    if (nullptr != g_error_hdl)
    {
        g_error_hdl(type, file, line, msg);
    }
    else
    {
        logDefaultErrorMsg(type, file, line, msg);
    }

    if (ErrorLevel::CRITICAL == type)
    {
        sb::debugBreak();
    }
}

void sb::internal::reportError(ErrorLevel type, char const * const file, u32 const line)
{
    if (nullptr != g_error_hdl)
    {
        g_error_hdl(type, file, line, "");
    }
    else
    {
        logDefaultErrorMsg(type, file, line, ERROR_DEFAULT_MSG[getEnumValue(type)]);
    }

    if ((ErrorLevel::CRITICAL == type) ||
        (ErrorLevel::WARNING == type))
    {
        sb::debugBreak();
    }
}

void sb::internal::reportNotImplemented(ErrorLevel type, char const * const file, u32 const line, char const * msg)
{
    char msg_fmt[255];
    sb::stringFormat(msg_fmt, "Not implemented: '{}'", msg);
    reportError(type, file, line, msg_fmt);
}

void sb::setErrorHandler(ErrorHandler const & hdl)
{
    g_error_hdl = hdl;
}

#endif
