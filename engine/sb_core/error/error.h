#pragma once

#include <sb_core/core.h>

namespace sb {

enum class ErrorLevel : u8
{
    CRITICAL,   // Unhandled critical error leading to a crash
    WARNING,    // ErrorLevel with side effect(s) affecting progam's behavior (graphic glitch,
                // bugs, etc.)
    NOTICE      // The error has been properly handled
};

} // namespace sb

// ErrorLevel::CRITICAL
#define sb_assert(cond, ...) sb_assert_internal(cond, ##__VA_ARGS__)
#define sb_report_error(...) sb_assert_internal(false, ##__VA_ARGS__)

// ErrorLevel::WARNING
#define sb_warn(cond, ...) sb_warn_internal(cond, ##__VA_ARGS__)
#define sb_report_warning(...) sb_warn_internal(false, ##__VA_ARGS__)
#define sb_not_implemented(str) sb_not_implemented_internal(str)

// ErrorLevel::NOTICE
#define sb_expect(cond, ...) sb_expect_internal(cond, ##__VA_ARGS__)
#define sb_dont_expect(cond, ...) sb_dont_expect_internal(cond, ##__VA_ARGS__)
#define sb_report_notice(...) sb_expect_internal(false, ##__VA_ARGS__)

#include <sb_core/_impl/error/error.h>
