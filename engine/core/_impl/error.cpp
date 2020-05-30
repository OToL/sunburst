#include <core/error.h>
#include <core/log.h>
#include <core/string/string_format.h>
#include <core/compiler.h>
#include <core/enum.h>

#if sbCTFIsEnabled(ERROR_FACILITY)

#    include <libc++/cassert>
#    include <libc++/cstdio>

namespace sb::detail {

static ErrorHandler gs_error_hdl = nullptr;
static void * gs_error_user_data = nullptr;

static char const * const ERROR_DEFAULT_MSG[] = {
    "Critical error detected", "Unhandled error detected", "Non fatal error detected"};

void logDefaultErrorMsg(ErrorType type, char const * const file, ui32 const line, char const * msg)
{
    char fmt_msg[255];
    sb::stringFormat(fmt_msg, "{} ({}, {})", msg, file, line);

    switch (type)
    {
        case ErrorType::CRITICAL:
        {
            sbLogE(fmt_msg);

            break;
        }

        case ErrorType::WARNING:
        case ErrorType::NOTICE:
        {
            sbLogW(fmt_msg);

            break;
        }
    }
}

void reportError(ErrorType type, char const * const file, ui32 const line, char const * msg)
{
    if (nullptr != gs_error_hdl)
    {
        gs_error_hdl(gs_error_user_data, type, file, line, msg);
    }
    else
    {
        logDefaultErrorMsg(type, file, line, msg);
    }

    if (ErrorType::CRITICAL == type)
    {
        sb::debugBreak();
    }
}

void reportError(ErrorType type, char const * const file, ui32 const line)
{
    if (nullptr != gs_error_hdl)
    {
        gs_error_hdl(gs_error_user_data, type, file, line, "");
    }
    else
    {
        logDefaultErrorMsg(type, file, line, ERROR_DEFAULT_MSG[getEnumValue(type)]);
    }

    if (ErrorType::CRITICAL == type)
    {
        sb::debugBreak();
    }
}

void reportNotImplemented(ErrorType type, char const * const file, ui32 const line,
                          char const * msg)
{
    char msg_fmt[255];
    sb::stringFormat(msg_fmt, "Not implemented: '{}'", msg);
    reportError(type, file, line, msg_fmt);
}

void setErrorHandler(ErrorHandler const & hdl, void * user_data)
{
    gs_error_hdl = hdl;
    gs_error_user_data = user_data;
}

} // namespace sb::detail

#endif
