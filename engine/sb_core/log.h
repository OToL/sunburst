#pragma once

#include <sb_core/core.h>

#include <sb_std/inplace_function>

namespace sb {

enum class LogLevel : ui8
{
    ERROR, // Important message to read & solve e.g. crash, out of memory, etc.
    WARNING, // Message about a problem which may impact application behavior e.g.
             // performance hitch, too many alive objects, etc.
    INFO, // Useful information regarding high level application logic e.g. state
          // transition, saving, etc.
    DEBUG // Verbose logging for debug purpose
};

using LogHandler = sbstdx::inplace_function<void(LogLevel lvl, char const * const file,
                                                 ui32 const line, char const * const msg)>;

void setLogHandler(LogHandler const & hdl);
void setLogQuiet(b8 quiet);
void setLogMinLevel(LogLevel min_level);

} // namespace sb

#define sbLog(lvl, msg, ...) sbLogImpl(lvl, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define sbLogE(msg, ...) sbLog(::sb::LogLevel::ERROR, msg, ##__VA_ARGS__)
#define sbLogW(msg, ...) sbLog(::sb::LogLevel::WARNING, msg, ##__VA_ARGS__)
#define sbLogI(msg, ...) sbLog(::sb::LogLevel::INFO, msg, ##__VA_ARGS__)
#define sbLogD(msg, ...) sbLog(::sb::LogLevel::DEBUG, msg, ##__VA_ARGS__)

#include "_impl/log.h"
