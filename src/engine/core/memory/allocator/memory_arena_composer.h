#pragma once

#include <core/memory/memory_arena.h>
#include <core/error.h>

namespace sb {

template <typename TProvider, typename TParent>
class MemoryArenaComposer : private TProvider, private TParent
{
    sbCopyProtect(MemoryArenaComposer);

    using ProviderInit = typename TProvider::InitParams;
    using ParentInit = typename TParent::InitParams;

    ParentInit patchInit(size_t arena_size, ParentInit const init)
    {
        sbAssert(0 != arena_size);
        sbAssert(init.m_arena.isEmpty());

        MemoryArena arena = {TProvider::allocate(arena_size, TParent::ALIGNMENT), arena_size};
        sbAssert(!arena.isEmpty());

        auto newInit{init};
        newInit.m_arena = arena;

        return newInit;
    }

public:
    struct InitParams
    {
        usize m_size;
        ProviderInit m_provider;
        ParentInit m_adaptee;
    };

    static constexpr usize ALIGNMENT = TParent::ALIGNMENT;

    MemoryArenaComposer(InitParams const & init = InitParams{})
        : TProvider(init.m_provider)
        , TParent(patchInit(init.m_size, init.m_adaptee))
    {
    }

    MemoryArenaComposer(usize size, ProviderInit provider_init, ParentInit adaptee_init)
        : TProvider(provider_init)
        , TParent(patchInit(size, adaptee_init))
    {
    }

    ~MemoryArenaComposer()
    {
        TProvider::deallocate(TParent::getArena().m_ptr);
    }

    MemoryArena getArena() const
    {
        return TParent::getArena();
    }

    using TParent::allocate;
    using TParent::deallocate;
    using TParent::deallocateAll;
    using TParent::getAlignment;
    using TParent::owns;
};

} // namespace sb
