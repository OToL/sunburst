#pragma once

#include "status_code.h"

namespace sb {

struct Status
{
    StatusCode code;
    char const * message;
};

inline b8 status_isOk(Status status)
{
    return status.code == StatusCode::OK;
}

} // namespace sb
