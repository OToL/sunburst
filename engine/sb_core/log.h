#pragma once

#include <sb_core/core.h>

namespace sb {

enum class LogLevel : u8
{
    ERROR, // Important message to read & solve e.g. crash, out of memory, etc.
    WARNING, // Message about a problem which may impact application behavior e.g.
             // performance hitch, too many alive objects, etc.
    INFO, // Useful information regarding high level application logic e.g. state
          // transition, saving, etc.
    DIAGNOSTIC // Verbose logging for debug purpose
};

void setLogQuiet(b8 quiet);
void setLogMinLevel(LogLevel min_level);

} // namespace sb

#define sbLog(lvl, msg, ...) sbLogImpl(lvl, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define sbLogE(msg, ...) sbLog(::sb::LogLevel::ERROR, msg, ##__VA_ARGS__)
#define sbLogW(msg, ...) sbLog(::sb::LogLevel::WARNING, msg, ##__VA_ARGS__)
#define sbLogI(msg, ...) sbLog(::sb::LogLevel::INFO, msg, ##__VA_ARGS__)
#define sbLogD(msg, ...) sbLog(::sb::LogLevel::DIAGNOSTIC, msg, ##__VA_ARGS__)

#include "_impl/log.h"
