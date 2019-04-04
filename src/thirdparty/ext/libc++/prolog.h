#pragma once

#define SB_SRC_STD_NS std
#define SB_SRC_STD_INC(file) <file>

#define SB_STD_NS wstd
#define SB_STD_EXT_NS ext

#define SB_STD_USE(entity) using ::SB_SRC_STD_NS::entity
#define SB_STD_PRIVATE_USE(entity) namespace detail { using ::SB_SRC_STD_NS::entity; }

namespace SB_STD_NS {
    namespace SB_STD_EXT_NS {
    }
}

namespace wstdx = wstd::ext;
