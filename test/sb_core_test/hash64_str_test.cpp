#include <sb_core/hash/hash64_str.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;
 
static constexpr char const * const HASH_STR_TEST_STRING = "Hello Hash";

TEST_CASE("Hash64Str default ctor", "[hash64_str]")
{
    Hash64Str const hash = {};
    REQUIRE_FALSE(isValid(hash));
}

TEST_CASE("Set Hash64Str from string", "[hash64_str]")
{
    Hash64Str const hash(HASH_STR_TEST_STRING);

    REQUIRE(hash.value == computeHash64(HASH_STR_TEST_STRING));
    REQUIRE(isValid(hash));
}

TEST_CASE("Set Hash64Str from value", "[hash64_str]")
{
    ui64 const HASH_TEST_VALUE = computeHash64(HASH_STR_TEST_STRING);
    Hash64Str const hash(HASH_STR_TEST_STRING);

    REQUIRE(hash.value == HASH_TEST_VALUE);
    REQUIRE(isValid(hash));
}

TEST_CASE("Hash64Str assignment", "[hash64_str]")
{
    Hash64Str hash(HASH_STR_TEST_STRING);
    Hash64Str const hash2("Test");

    REQUIRE(hash != hash2);

    hash = hash2;

    REQUIRE(hash == hash2);
}

TEST_CASE("Hash64Str constexpr check", "[hash64_str]")
{
    STATIC_REQUIRE((computeHash64(HASH_STR_TEST_STRING) == Hash64Str(HASH_STR_TEST_STRING).value));
}

TEST_CASE("Hash64Str literal", "[hash64_str]")
{
   STATIC_REQUIRE((computeHash64(HASH_STR_TEST_STRING) == "Hello Hash"_h64s.value));
}

#include <catch2/test_epilog.h>
