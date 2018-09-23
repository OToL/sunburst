#pragma once

#pragma clang diagnostic push

#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma clang diagnostic ignored "-Wshift-sign-overflow"
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wdeprecated"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"

#include <gtest/gtest.h>

#pragma clang diagnostic pop

#include <libc++/iterator>
#include <libc++/algorithm>

#define EXPECT_FLOAT_SEQ_EQ(expected, actual)                                                                            \
    {                                                                                                                    \
        usize const expectSize = wstd::distance(wstd::begin(expected), wstd::end(expected));                            \
        usize const actualSize = wstd::distance(wstd::begin(actual), wstd::end(actual));                                \
        EXPECT_EQ(expectSize, actualSize);                                                                               \
        if (expectSize == actualSize)                                                                                    \
        {                                                                                                                \
            auto actualIter = wstd::begin(actual);                                                                       \
            auto expectIter = wstd::begin(expected);                                                                     \
            for (; (actualIter != wstd::end(actual)) && (expectIter != wstd::end(expected)); ++actualIter, ++expectIter) \
            {                                                                                                            \
                EXPECT_FLOAT_EQ(*expectIter, *actualIter);                                                               \
            }                                                                                                            \
        }                                                                                                                \
    }


namespace gtest { namespace ext {

}}

namespace gtestx = gtest::ext;
