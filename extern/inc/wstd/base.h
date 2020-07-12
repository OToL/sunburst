#pragma once

#define SB_SRC_STD_NS std
#define SB_SRC_STD_INC(file) <file>

#define SB_WSTD_NS wstd
#define SB_WSTD_EXT_NS ext

#define SB_SRC_STD_USE(entity) using ::SB_SRC_STD_NS::entity

namespace SB_WSTD_NS {
    namespace SB_WSTD_EXT_NS {
    }
}

namespace wstdx = wstd::ext;
