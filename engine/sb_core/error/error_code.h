#pragma once

#include <sb_core/core.h>

namespace sb {

enum class ErrorCode : u32
{
    OK,
    CANCELLED,
    INVALID_ARGUMENT,
    DEADLINE_EXCEEDED,
    NOT_FOUND,
    ALREADY_EXISTS,
    PERMISSION_DENIED,
    UNAUTHENTICATED,
    RESOURCE_EXHAUSTED,
    FAILED_PRECONDITION,
    ABORTED,
    OUT_OF_RANGE,
    UNIMPLEMENTED,
    INTERNAL,
    UNAVAILABLE,
    DATA_LOSS,
    UNKNOWN
};

char const * toString(ErrorCode status_code);

} // namespace sb
