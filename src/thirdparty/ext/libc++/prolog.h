#pragma once

#define SB_SRC_STD_NS std
#define SB_SRC_STD_INC(file) <file>

#define SB_STD_NS_BEGIN namespace wstd {
#define SB_STD_NS_END }

#define SB_STD_EXT_NS_BEGIN namespace ext {
#define SB_STD_EXT_NS_END }

#define SB_STD_USE(construct) using ::SB_SRC_STD_NS::construct

#define SB_STD_PRIVATE_USE(construct) namespace priv { using ::SB_SRC_STD_NS::construct; }

SB_STD_NS_BEGIN
    SB_STD_EXT_NS_BEGIN
    SB_STD_EXT_NS_END
SB_STD_NS_END

namespace wstdx = wstd::ext;
