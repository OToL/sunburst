#pragma once

#include <sb_core/core.h>

namespace sb {

enum class ErrorLevel : u8
{
    CRITICAL, // Unhandled critical error leading to a crash
    WARNING, // ErrorLevel with side effect(s) affecting progam's behavior (graphic glitch,
             // bugs, etc.)
    NOTICE // The error has been properly handled
};

} // namespace sb

// ErrorLevel::CRITICAL
#define sbAssert(cond, ...) sbAssertInternal(cond, ##__VA_ARGS__)

// ErrorLevel::WARNING
#define sbWarn(cond, ...) sbWarnInternal(cond, ##__VA_ARGS__)
#define sbNotImplemented(str) sbNotImplementedInternal(str)

// ErrorLevel::NOTICE
#define sbExpect(cond, ...) sbExpectInternal(cond, ##__VA_ARGS__)
#define sbDontExpect(cond, ...) sbDontExpectInternal(cond, ##__VA_ARGS__)

#include "_impl/error.h"
