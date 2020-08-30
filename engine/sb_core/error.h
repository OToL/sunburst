#pragma once

#include <sb_base/base.h>

#include <sb_std/inplace_function>

namespace sb {

enum class ErrorLevel : ui8
{
    CRITICAL, // Unhandled critical error leading to a crash
    WARNING, // ErrorLevel with side effect(s) affecting progam's behavior (graphic glitch,
             // bugs, etc.)
    NOTICE // The error has been properly handled
};

using ErrorHandler = sbstdx::inplace_function<void(ErrorLevel type, char const * file,
                                                   ui32 const line, char const * msg)>;

void setErrorHandler(ErrorHandler const & hdl);

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
