#pragma once

#include <core/types.h>

namespace sb {

struct MemoryArena
{
    b8 isInRange(void const * ptr, size_t size) const
    {
        return ((m_ptr <= ptr) && ((static_cast<ui8 const *>(m_ptr) + m_size) >=
                                   (static_cast<ui8 const *>(ptr) + size)));
    }

    b8 isInRange(void const * ptr) const
    {
        return ((m_ptr <= ptr) && ((static_cast<ui8 const *>(m_ptr) + m_size) > ptr));
    }

    b8 isEmpty() const
    {
        return ((0 == m_size) || (nullptr == m_ptr));
    }

    void * m_ptr = nullptr;
    usize m_size = 0;
};

} // namespace sb
