#include <sb_core/log.h>
#include <sb_core/os.h>
#include <sb_core/error.h>
#include <sb_core/hook.h>
#include <sb_core/string/format.h>
#include <sb_core/memory/memory_arena.h>

#include <sb_slw/iterator>

static sb::LogHook g_log_hdl;

static char const * const LOG_TYPE_TO_STR[] = {"ERROR", "WARNING", "INFO", "DEBUG"};

void sb::internal::logMessage(LogLevel type, char const * file, u32 line, char const * msg)
{
    sb_assert((u8)type < slw::size(LOG_TYPE_TO_STR));

    if (nullptr != g_log_hdl)
    {
        g_log_hdl(type, file, line, msg);
    }
    else
    {
        char tmp_msg[1024];
        sb::formatString(tmp_msg, "[{}] {}\n", LOG_TYPE_TO_STR[(u8)type], msg);

        outputDebugString(&tmp_msg[0]);
    }
}

void sb::setLogHook(LogHook const & hdl)
{
    g_log_hdl = hdl;
}
