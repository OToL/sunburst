#include <core/log.h>
#include <core/os.h>
#include <core/string/string_format.h>

#include <libc++/iterator>

namespace sb::detail {

static LogHandler gs_log_hdl = nullptr;
static void * gs_log_user_data = nullptr;

static char const * const LOG_TYPE_TO_STR[] = {"ERROR", "WARNING", "INFO", "DEBUG"};

void logMessage(LogLevel type, char const * file, ui32 line, char const * msg)
{
    sbAssert((ui8)type < wstd::size(LOG_TYPE_TO_STR));

    if (nullptr != gs_log_hdl)
    {
        gs_log_hdl(gs_log_user_data, type, file, line, msg);
    }
    else
    {
        char tmp_msg[255];
        sb::stringFormat(tmp_msg, "[{}] {}\n", LOG_TYPE_TO_STR[(ui8)type], msg);

        outputDebugString(tmp_msg);
    }
}

void setLogHandler(LogHandler const & hdl, void * user_data)
{
    gs_log_hdl = hdl;
    gs_log_user_data = user_data;
}

} // namespace sb
