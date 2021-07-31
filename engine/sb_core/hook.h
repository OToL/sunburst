#pragma once

#include <sb_core/error/error.h>
#include <sb_core/error/error_code.h>
#include <sb_core/log.h>

#include <sb_std/inplace_function>

namespace sb {

using ErrorHook = sbstdx::inplace_function<void(ErrorLevel type, char const * file, u32 const line,
                                                ErrorCode status_code, char const * msg)>;

using LogHook =
    sbstdx::inplace_function<void(LogLevel lvl, char const * const file, u32 const line, char const * const msg)>;

void setLogHook(LogHook const & hdl);
void setErrorHook(ErrorHook const & hdl);

} // namespace sb
