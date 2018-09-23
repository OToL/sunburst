#pragma once

#if defined(SB_TARGET_MACOS)
#    include "_pimpl/macos/compiler_macos.h"
#else
#    error "Unsupported compiler"
#endif

#define sb_forceinline sb_forceinline_impl
#define sb_noinline sb_noinline_impl

#define sb_restrict sb_restrict_impl

#define sbBaseClass(...) \
    using BaseClass = __VA_ARGS__

#define sbCopyProtect(classname)                       \
    classname & operator=(classname const &) = delete; \
    classname(classname const &) = delete

#define sbConstructProtect(classname) \
    classname() = delete

#define sbConcatId(id1, id2) id1##id2

#define sbConcatStr(id1, id2) id1 id2

#define sbIdToStr(id) #id

#define sbFwCall(fn, ...) fn(__VA_ARGS__)

#define sbDebugBreak() sbDebugBreakImpl()

#define SB_CTF_DISABLED 1
#define SB_CTF_ENABLED 2

#define sbCTFIsEnabled(feature) (1 == 2 / SB_CTF_##feature)
#define sbCTVIsEq(feature, value) ((1 / SB_CTV_##feature + 1) && (SB_CTV_##feature == value))
