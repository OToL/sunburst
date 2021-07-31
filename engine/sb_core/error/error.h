#pragma once

#include <sb_core/core.h>

namespace sb {

enum class ErrorLevel : u8
{
    CRITICAL, // Unhandled critical error leading to a crash
    WARNING, // ErrorLevel with side effect(s) affecting progam's behavior (graphic glitch,
             // bugs, etc.)
    NOTICE // The error has been properly handled
};

} // namespace sb

// ErrorLevel::CRITICAL
#define sb_assert(cond, ...) sb_assert_internal(cond, ##__VA_ARGS__)

// ErrorLevel::WARNING
#define sb_warning(cond, ...) sb_warning_internal(cond, ##__VA_ARGS__)
#define sb_not_implemented(str) sb_not_implemented_internal(str)

// ErrorLevel::NOTICE
#define sb_expected(cond, ...) sb_expected_internal(cond, ##__VA_ARGS__)
#define sb_not_expected(cond, ...) sb_not_expected_internal(cond, ##__VA_ARGS__)

#include <sb_core/_impl/error/error.h>
