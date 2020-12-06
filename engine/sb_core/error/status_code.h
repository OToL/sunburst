#pragma once

#include <sb_core/core.h>

namespace sb {

enum class StatusCode : u32
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

char const * statuscode_getValueName(StatusCode status_code);

} // namespace sb
