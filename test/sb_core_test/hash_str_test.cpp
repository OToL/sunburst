#include <sb_core/hash/hash_str.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;
 
static constexpr char const * const HASH_STR_TEST_STRING = "Hello Hash";

TEST_CASE("HashStr default ctor", "[hashstr]")
{
    HashStr const hash{};
    REQUIRE_FALSE(hash.isValid());
}

TEST_CASE("Set HashStr from string", "[hashstr]")
{
    HashStr const hash(HASH_STR_TEST_STRING);

    REQUIRE(hash.value == HashStr<>::HashPolicy::compute(HASH_STR_TEST_STRING));
    REQUIRE(hash.isValid());
}

TEST_CASE("Set HashStr from value", "[hashstr]")
{
    HashStr<>::Value const HASH_TEST_VALUE = HashStr<>::HashPolicy::compute(HASH_STR_TEST_STRING);
    HashStr const hash(HASH_STR_TEST_STRING);

    REQUIRE(hash.value == HASH_TEST_VALUE);
    REQUIRE(hash.isValid());
}

TEST_CASE("Hashstr assignment", "[hashstr]")
{
    HashStr hash(HASH_STR_TEST_STRING);
    HashStr const hash2("Test");

    REQUIRE(hash != hash2);

    hash = hash2;

    REQUIRE(hash == hash2);
}

TEST_CASE("Constexpr check", "[hashstr]")
{
    STATIC_REQUIRE((HashStr<>::HashPolicy::compute(HASH_STR_TEST_STRING) == HashStr(HASH_STR_TEST_STRING).value));
}

#include <catch2/test_epilog.h>
