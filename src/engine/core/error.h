#pragma once

#include "platform.h"

namespace sb {

enum class ErrorType : ui8
{
    CRITICAL,   // The application will crash
    WARNING,    // The application may misbehave (glitches, bugs, etc.)
    NOTICE      // The application should run fine
};

using ErrorHandler = void (*)(void * user_data, ErrorType type, char const * file, ui32 const line, char const * msg);

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
