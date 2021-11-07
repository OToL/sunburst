#include <sb_core/error/error_code.h>
#include <sb_core/error/error.h>
#include <sb_core/cast.h>

constexpr char const * ERROR_CODE_TO_STRING[] = {"OK",
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
                                              "UNKNOWN"};

char const * sb::toString(ErrorCode status_code)
{
    sb_assert(integral_cast<>(status_code) < sizeof(ERROR_CODE_TO_STRING));

    return ERROR_CODE_TO_STRING[integral_cast<>(status_code)];
}
