#pragma once

namespace sb::internal {

struct LayerFile
{
    void * value;
};

inline b8 layerfile_isValid(LayerFile hdl)
{
    return nullptr != hdl.value;
}
} // namespace sb::internal
