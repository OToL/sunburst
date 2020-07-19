#pragma once

#include "base.h"

namespace sb {

enum class ErrorType : ui8
{
    CRITICAL, // Unhandled critical error leading to a crash
    WARNING, // Error with side effect(s) affecting progam's behavior (graphic glitch,
             // bugs, etc.)
    NOTICE // The error has been properly handled
};

using ErrorHandler = void (*)(void * user_data, ErrorType type, char const * file, ui32 const line,
                              char const * msg);

void setErrorHandler(ErrorHandler const & hdl, void * user_data = nullptr);

} // namespace sb

// ErrorType::CRITICAL
#define sbAssert(cond, ...) sbAssertInternal(cond, ##__VA_ARGS__)

// ErrorType::WARNING
#define sbWarn(cond, ...) sbWarnInternal(cond, ##__VA_ARGS__)
#define sbNotImplemented(str) sbNotImplementedInternal(str)

// ErrorType::NOTICE
#define sbExpect(cond, ...) sbExpectInternal(cond, ##__VA_ARGS__)
#define sbExpectFalse(cond, ...) sbExpectFalseInternal(cond, ##__VA_ARGS__)

#include "_impl/error.h"
