#pragma once

#include "../../inc/libc++/prolog.h"

#define SB_STD_EXT_NS_BEGIN namespace ext {
#define SB_STD_EXT_NS_END }

SB_STD_NS_BEGIN
    SB_STD_EXT_NS_BEGIN
    SB_STD_EXT_NS_END
SB_STD_NS_END

namespace wstdx = wstd::ext;
