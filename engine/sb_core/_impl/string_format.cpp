#include <sb_core/string/string_format.h>
#include <sb_core/string/string_utility.h>
#include <sb_core/error/error.h>

#include <sb_std/cctype>
#include <sb_std/type_traits>

sb::usize sb::internal::stringFormat(sbstd::span<char> dest_buffer, char const * const format, sbstd::span<FmtArg> args)
{
    usize copied_bytes = 0;

    if (!dest_buffer.empty())
    {
        char * dest_iter = dest_buffer.data();
        sbAssert(nullptr != dest_iter);

        auto const dest_capacity = numericConv<usize>(dest_buffer.size());

        char const * format_iter = format;
        s32 lastParamIdx = 0;

        while ((0 != *format_iter) && (copied_bytes < dest_capacity))
        {
            if ('{' == *format_iter)
            {
                char const next_token = format_iter[1];

                if ('{' == next_token)
                {
                    ++format_iter;
                }
                else if (sbDontExpect(0 == next_token, "Reached end of format string without finding "
                                                       "argument closing '}'"))
                {
                    break;
                }
                else
                {
                    s32 arg_idx = -1;

                    // Argument auto index increment '{}'
                    if ('}' == next_token)
                    {
                        format_iter += 2;

                        if (sbExpect(-1 != lastParamIdx, "You cannot mix indexed and auto-increment arguments"))
                        {
                            arg_idx = lastParamIdx++;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    // Argument specification
                    else if (sbExpect(sbstd::isdigit(next_token), "Only digit is allowed in argument specification") &&
                             sbExpect(0 >= lastParamIdx, "You cannot mix indexed and auto-increment "
                                                         "argument specification") &&
                             sbExpect('}' == format_iter[2], "Format argument index must be within [0-9] range"))
                    {
                        arg_idx = next_token - '0';
                        lastParamIdx = -1;
                        format_iter += 3;
                    }

                    // Skip the argument specification
                    if (arg_idx == -1)
                    {
                        char const * const closing_arg = strchr(format_iter + 1, '}');

                        if (sbExpect(nullptr != closing_arg, "Cannot find closing argument '}'"))
                        {
                            format_iter = closing_arg + 1;
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }

                    copied_bytes += args[(u32)arg_idx].fmt_cb(
                        args[(u32)arg_idx].value, sbstd::span<char>{dest_iter, dest_capacity - copied_bytes});
                    dest_iter = dest_buffer.data() + copied_bytes;

                    continue;
                }
            }
            else if ('}' == *format_iter)
            {
                ++format_iter;

                if (sbDontExpect('}' != *format_iter, "'}' found without former '{'"))
                {
                    continue;
                }
            }

            *dest_iter = *format_iter;
            ++copied_bytes;

            ++dest_iter;
            ++format_iter;
        }

        *dest_iter = '\0';
    }

    return copied_bytes;
}
