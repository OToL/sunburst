#include <core/string/string_format.h>
#include <core/string/utility.h>

#include <wstd/cctype>
#include <wstd/type_traits>

namespace sb::detail {

usize stringFormat(wstd::span<char> dest_buffer, char const * const format, wstd::span<FmtArg> args)
{
    usize copied_bytes = 0;

    if (!dest_buffer.empty())
    {
        char * dest_iter = dest_buffer.data();
        sbAssert(nullptr != dest_iter);

        usize const dest_capacity = numericConv<usize>(dest_buffer.size());

        char const * format_iter = format;
        si32 lastParamIdx = 0;

        while ((0 != *format_iter) && (copied_bytes < dest_capacity))
        {
            if ('{' == *format_iter)
            {
                char const next_token = format_iter[1];

                if ('{' == next_token)
                {
                    ++format_iter;
                }
                else if (sbExpectFalse(0 == next_token,
                                       "Reached end of format string without finding "
                                       "argument closing '}'"))
                {
                    break;
                }
                else
                {
                    si32 arg_idx = -1;

                    // Argument auto index increment '{}'
                    if ('}' == next_token)
                    {
                        format_iter += 2;

                        if (sbExpectTrue(-1 != lastParamIdx,
                                         "You cannot mix indexed and auto-increment arguments"))
                        {
                            arg_idx = lastParamIdx++;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    // Argument specification
                    else if (sbExpectTrue(wstd::isdigit(next_token),
                                          "Only digit is allowed in argument specification") &&
                             sbExpectTrue(0 >= lastParamIdx,
                                          "You cannot mix indexed and auto-increment "
                                          "argument specification") &&
                             sbExpectTrue('}' == format_iter[2],
                                          "Format argument index must be within [0-9] range"))
                    {
                        arg_idx = next_token - '0';
                        lastParamIdx = -1;
                        format_iter += 3;
                    }

                    // Skip the argument specification
                    if (arg_idx == -1)
                    {
                        char const * const closing_arg = strchr(format_iter + 1, '}');

                        if (sbExpectTrue(nullptr != closing_arg,
                                         "Cannot find closing argument '}'"))
                        {
                            format_iter = closing_arg + 1;
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }

                    copied_bytes += args[(ui32)arg_idx].m_fmt_cb(
                        args[(ui32)arg_idx].m_value,
                        wstd::span<char>{dest_iter,
                                         dest_capacity - copied_bytes});
                    dest_iter = dest_buffer.data() + copied_bytes;

                    continue;
                }
            }
            else if ('}' == *format_iter)
            {
                ++format_iter;

                if (sbExpectFalse('}' != *format_iter, "'}' found without former '{'"))
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

} // namespace sb::detail
