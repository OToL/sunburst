#pragma once

#ifndef TARGET_CPU_X86_64
#    error "not supported"
#endif

namespace sb::detail {

constexpr char const * const PHYSICAL_PATH_SEPARATOR = "/";
constexpr size_t const PHYSICAL_PATH_MAX_LEN = 255;

} // namespace sb::detail
