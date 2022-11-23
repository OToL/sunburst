#pragma once

#include <sb_core/error.h>
#include <sb_core/log.h>

#include <sb_slw/inplace_function>

namespace sb {

using ErrorHook = slwx::inplace_function<void(ErrorLevel type, char const * file, u32 const line, ErrorCode status_code, char const * msg)>;

using LogHook = slwx::inplace_function<void(LogLevel lvl, char const * const file, u32 const line, char const * const msg)>;

void setLogHook(LogHook const & hdl);
void setErrorHook(ErrorHook const & hdl);

} // namespace sb
