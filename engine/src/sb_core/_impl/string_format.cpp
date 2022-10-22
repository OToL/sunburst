#include <sb_core/string/format.h>
#include <sb_core/string/utility.h>
#include <sb_core/error.h>

#include <sb_slw/cctype>
#include <sb_slw/type_traits>

sb::usize sb::internal::formatString(slw::span<char> dest_buffer, char const * const format, slw::span<FmtArg> args)
{
    usize copied_bytes = 0;

    if (!dest_buffer.empty())
    {
        char * dest_iter = dest_buffer.data();
        sb_assert(nullptr != dest_iter);

        auto const dest_capacity = integral_cast<usize>(dest_buffer.size());

        char const * format_iter = format;
        i32 lastParamIdx = 0;

        while ((0 != *format_iter) && (copied_bytes < dest_capacity))
        {
            if ('{' == *format_iter)
            {
                char const next_token = format_iter[1];

                if ('{' == next_token)
                {
                    ++format_iter;
                }
                else if (sb_dont_expect(0 == next_token, "Reached end of format string without finding "
                                                       "argument closing '}'"))
                {
                    break;
                }
                else
                {
                    i32 arg_idx = -1;

                    // Argument auto index increment '{}'
                    if ('}' == next_token)
                    {
                        format_iter += 2;

                        if (sb_expect(-1 != lastParamIdx, "You cannot mix indexed and auto-increment arguments"))
                        {
                            arg_idx = lastParamIdx++;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    // Argument specification
                    else if (sb_expect(slw::isdigit(next_token), "Only digit is allowed in argument specification") &&
                             sb_expect(0 >= lastParamIdx, "You cannot mix indexed and auto-increment "
                                                         "argument specification") &&
                             sb_expect('}' == format_iter[2], "Format argument index must be within [0-9] range"))
                    {
                        arg_idx = next_token - '0';
                        lastParamIdx = -1;
                        format_iter += 3;
                    }

                    // Skip the argument specification
                    if (arg_idx == -1)
                    {
                        char const * const closing_arg = strchr(format_iter + 1, '}');

                        if (sb_expect(nullptr != closing_arg, "Cannot find closing argument '}'"))
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
                        args[(u32)arg_idx].value, slw::span<char>{dest_iter, dest_capacity - copied_bytes});
                    dest_iter = dest_buffer.data() + copied_bytes;

                    continue;
                }
            }
            else if ('}' == *format_iter)
            {
                ++format_iter;

                if (sb_dont_expect('}' != *format_iter, "'}' found without former '{'"))
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
