#include <sb_core/log.h>
#include <sb_core/system.h>
#include <sb_core/error/error.h>
#include <sb_core/hook.h>
#include <sb_core/string/string_format.h>

#include <sb_std/iterator>

static sb::LogHook g_log_hdl;

static char const * const LOG_TYPE_TO_STR[] = {"ERROR", "WARNING", "INFO", "DEBUG"};

void sb::internal::logMessage(LogLevel type, char const * file, u32 line, char const * msg)
{
    sb_assert((u8)type < sbstd::size(LOG_TYPE_TO_STR));

    if (nullptr != g_log_hdl)
    {
        g_log_hdl(type, file, line, msg);
    }
    else
    {
        char tmp_msg[1024];
        sb::stringFormat(tmp_msg, "[{}] {}\n", LOG_TYPE_TO_STR[(u8)type], msg);

        outputDebugString(&tmp_msg[0]);
    }
}

void sb::setLogHook(LogHook const & hdl)
{
    g_log_hdl = hdl;
}
