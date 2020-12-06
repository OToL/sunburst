#include <sb_core/hash/hash32_str.h>

#include <catch2/test_prolog.h>
#include <catch2/xcatch.hpp>

using namespace sb;

static constexpr char const * const HASH_STR_TEST_STRING = "Hello Hash";

TEST_CASE("Hash32Str default ctor", "[hash32_str]")
{
    Hash32Str const hash = {};
    REQUIRE_FALSE(hash32str_isValid(hash));
}

TEST_CASE("Set Hash32Str from string", "[hash32_str]")
{
    Hash32Str const hash(makeHash32Str(HASH_STR_TEST_STRING));

    REQUIRE(hash.value == computeHash32(HASH_STR_TEST_STRING));
    REQUIRE(hash32str_isValid(hash));
}

TEST_CASE("Set Hash32Str from value", "[hash32_str]")
{
    u32 const HASH_TEST_VALUE = computeHash32(HASH_STR_TEST_STRING);
    Hash32Str const hash(makeHash32Str(HASH_STR_TEST_STRING));

    REQUIRE(hash.value == HASH_TEST_VALUE);
    REQUIRE(hash32str_isValid(hash));
}

TEST_CASE("Hash32Str assignment", "[hash32_str]")
{
    Hash32Str hash(makeHash32Str(HASH_STR_TEST_STRING));
    Hash32Str const hash2("Test"_h32s);

    REQUIRE(hash != hash2);

    hash = hash2;

    REQUIRE(hash == hash2);
}

TEST_CASE("Hash32Str constexpr check", "[hash32_str]")
{
    STATIC_REQUIRE((computeHash32(HASH_STR_TEST_STRING) == makeHash32Str(HASH_STR_TEST_STRING).value));
}

TEST_CASE("Hash32Str literal", "[hash32_str]")
{
    STATIC_REQUIRE((computeHash32(HASH_STR_TEST_STRING) == "Hello Hash"_h32s.value));
}

#include <catch2/test_epilog.h>
