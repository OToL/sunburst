#include <sb_core/error/status_code.h>
#include <sb_core/error/error.h>
#include <sb_core/enum.h>

constexpr char const * STATUS_CODE_NAMES [] =
{
    "OK",
    "CANCELLED",
    "INVALID_ARGUMENT",
    "DEADLINE_EXCEEDED",
    "NOT_FOUND",
    "ALREADY_EXISTS",
    "PERMISSION_DENIED",
    "UNAUTHENTICATED",
    "RESOURCE_EXHAUSTED",
    "FAILED_PRECONDITION",
    "ABORTED",
    "OUT_OF_RANGE",
    "UNIMPLEMENTED",
    "INTERNAL",
    "UNAVAILABLE",
    "DATA_LOSS",
    "UNKNOWN"
};

char const * sb::getAsString(StatusCode status_code)
{
    sbAssert(getEnumValue(status_code) < sizeof(STATUS_CODE_NAMES));

    return STATUS_CODE_NAMES[getEnumValue(status_code)];
}
