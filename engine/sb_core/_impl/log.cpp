#include <sb_core/log.h>
#include <sb_core/os.h>
#include <sb_core/string/string_format.h>

#include <sb_std/iterator>

namespace sb::internal {

static LogHandler g_log_hdl;

static char const * const LOG_TYPE_TO_STR[] = {"ERROR", "WARNING", "INFO", "DEBUG"};

void logMessage(LogLevel type, char const * file, ui32 line, char const * msg)
{
    sbAssert((ui8)type < sbstd::size(LOG_TYPE_TO_STR));

    if (nullptr != g_log_hdl)
    {
        g_log_hdl(type, file, line, msg);
    }
    else
    {
        char tmp_msg[255];
        sb::stringFormat(tmp_msg, "[{}] {}\n", LOG_TYPE_TO_STR[(ui8)type], msg);

        outputDebugString(tmp_msg);
    }
}

void setLogHandler(LogHandler const & hdl)
{
    g_log_hdl = hdl;
}

} // namespace sb::internal
