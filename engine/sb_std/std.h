#pragma once

#define SB_SRC_STD_NS std
#define SB_SRC_STD_INC(file) <file>

#define SB_STD_NS sbstd
#define SB_STD_NS_EXT ext

#define SB_SRC_STD_USE(entity) using ::SB_SRC_STD_NS::entity

namespace SB_STD_NS 
{
    namespace SB_STD_NS_EXT
    {
    }
}

namespace sbstdx = SB_STD_NS::SB_STD_NS_EXT;
