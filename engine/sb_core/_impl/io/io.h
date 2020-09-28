#pragma once

namespace sb::internal {

    struct LayerFileHdl
    {
        void * value;
    };

    inline b8 isValid(LayerFileHdl hdl)
    {
        return nullptr != hdl.value;
    }
}
