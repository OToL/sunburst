#pragma once

#include <sb_core/error/error.h>
#include <sb_core/error/status_code.h>
#include <sb_core/log.h>

#include <sb_std/inplace_function>

namespace sb {

using ErrorHandler = sbstdx::inplace_function<void(ErrorLevel type, char const * file, u32 const line,
                                                   StatusCode status_code, char const * msg)>;

using LogHandler =
    sbstdx::inplace_function<void(LogLevel lvl, char const * const file, u32 const line, char const * const msg)>;

void setLogHandler(LogHandler const & hdl);
void setErrorHandler(ErrorHandler const & hdl);

} // namespace sb
