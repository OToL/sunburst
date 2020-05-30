#pragma once

#include "types.h"

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
#define sbAssert(cond, ...) sbAssertImpl(cond, ##__VA_ARGS__)

// ErrorType::WARNING
#define sbWarn(cond, ...) sbWarnImpl(cond, ##__VA_ARGS__)
#define sbNotImplemented(str) sbNotImplementedImpl(str)

// ErrorType::NOTICE
#define sbExpectTrue(cond, ...) sbExpectTrueImpl(cond, ##__VA_ARGS__)
#define sbExpectFalse(cond, ...) sbExpectFalseImpl(cond, ##__VA_ARGS__)

#include "_impl/error.h"
