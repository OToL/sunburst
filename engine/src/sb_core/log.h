#pragma once

#include <sb_core/core.h>

namespace sb {

enum class LogLevel : u8
{
    ERROR,      // Important message to read & solve e.g. crash, out of memory, etc.
    WARNING,    // Message about a problem which may impact application behavior e.g.
                // performance hitch, too many alive objects, etc.
    INFO,       // Useful information regarding high level application logic e.g. state
                // transition, saving, etc.
    VERBOSE     // Verbose logging for debug purpose
};

void setLogQuiet(b8 quiet);
void setLogMinLevel(LogLevel min_level);

} // namespace sb

#define sb_log(lvl, msg, ...) sb_log_internal(lvl, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define sb_log_error(msg, ...) sb_log(::sb::LogLevel::ERROR, msg, ##__VA_ARGS__)
#define sb_log_warning(msg, ...) sb_log(::sb::LogLevel::WARNING, msg, ##__VA_ARGS__)
#define sb_log_info(msg, ...) sb_log(::sb::LogLevel::INFO, msg, ##__VA_ARGS__)
#define sb_log_verbose(msg, ...) sb_log(::sb::LogLevel::VERBOSE, msg, ##__VA_ARGS__)

#include "_impl/log.h"

