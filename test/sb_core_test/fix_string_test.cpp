#include <sb_core/string/fix_string.h>

#include <catch2/test_prolog.h>
#include <catch2/xcatch.hpp>

using namespace sb;

TEST_CASE("FixString default ctor", "[fix_string]")
{
    FixString<10> testStr;

    REQUIRE(testStr.empty());
    REQUIRE(testStr.length() == 0U);
    REQUIRE_THAT(testStr.c_str(), Catch::Matchers::Equals(""));
}

TEST_CASE("FixString ctor raw string", "[fix_string]")
{
    FixString<10> testStr("Hello");

    REQUIRE_FALSE(testStr.empty());
    REQUIRE(testStr.length() == 5U);
    REQUIRE_THAT(testStr.c_str(), Catch::Matchers::Equals("Hello"));
}

TEST_CASE("FixString copy ctor", "[fix_string]")
{
    FixString<10> testStr("Hello");
    FixString<10> testStr2(testStr); // NOLINT(performance-unnecessary-copy-initialization)

    REQUIRE_FALSE(testStr2.empty());
    REQUIRE(testStr2.length() == 5U);
    REQUIRE_THAT(testStr.c_str(), Catch::Matchers::Equals("Hello"));
}

TEST_CASE("FixString assign raw string", "[fix_string]")
{
    FixString<10> testStr;
    REQUIRE(testStr.empty());

    testStr = "Hello";
    REQUIRE_FALSE(testStr.empty());
    REQUIRE(testStr.length() == 5U);
    REQUIRE_THAT(testStr.c_str(), Catch::Matchers::Equals("Hello"));
}

TEST_CASE("FixString assign FixString", "[fix_string]")
{
    FixString<10> testStr("Hello");
    FixString<10> testStr2;

    REQUIRE_FALSE(testStr.empty());
    REQUIRE_THAT(testStr.c_str(), Catch::Matchers::Equals("Hello"));
    REQUIRE(testStr2.empty());

    testStr2 = testStr;

    REQUIRE_FALSE(testStr2.empty());
    REQUIRE(5U == testStr2.length());
    REQUIRE_THAT(testStr2.c_str(), Catch::Matchers::Equals("Hello"));
}

TEST_CASE("FixString assign raw string exceeded", "[fix_string]")
{
    FixString<3> testStr;
    REQUIRE(testStr.empty());

    testStr = "Hello";
    REQUIRE_FALSE(testStr.empty());
    REQUIRE(2U == testStr.length());
    REQUIRE_THAT(testStr.c_str(), Catch::Matchers::Equals("He"));
}

TEST_CASE("FixString assign FixString exceeded", "[fix_string]")
{
    FixString<10> testStr("Hello");
    FixString<3> testStr2;

    REQUIRE_FALSE(testStr.empty());
    REQUIRE_THAT(testStr.c_str(), Catch::Matchers::Equals("Hello"));
    REQUIRE(testStr2.empty());

    testStr2 = testStr;

    REQUIRE_FALSE(testStr2.empty());
    REQUIRE(2U == testStr2.length());
    REQUIRE_THAT(testStr2.c_str(), Catch::Matchers::Equals("He"));
}

TEST_CASE("FixString concat raw string", "[fix_string]")
{
    FixString<20> testStr("Hello ");
    testStr += "world";

    REQUIRE_THAT(testStr.c_str(), Catch::Matchers::Equals("Hello world"));
}

TEST_CASE("FixString concat raw string exceeded", "[fix_string]")
{
    FixString<9> testStr("Hello ");
    testStr += "world";

    REQUIRE_THAT(testStr.c_str(), Catch::Matchers::Equals("Hello wo"));
}

TEST_CASE("FixString concat FixString", "[fix_string]")
{
    FixString<20> testStr("Hello ");
    FixString<20> testStr2("world");
    testStr += testStr2;

    REQUIRE_THAT(testStr.c_str(), Catch::Matchers::Equals("Hello world"));
}

TEST_CASE("FixString concat FixString exceeded", "[fix_string]")
{
    FixString<9> testStr("Hello ");
    FixString<20> testStr2("world");
    testStr += testStr2;

    REQUIRE_THAT(testStr.c_str(), Catch::Matchers::Equals("Hello wo"));
}

TEST_CASE("FixString back", "[fix_string]")
{
    FixString<10> testStr("Hello");

    REQUIRE(testStr.back() == 'o');
}

TEST_CASE("FixString push_back", "[fix_string]")
{
    FixString<10> testStr("/hello");

    REQUIRE(testStr.back() == 'o');
    REQUIRE(testStr.length() == 6ULL);

    testStr.push_back('/');

    REQUIRE(testStr.back() == '/');
    REQUIRE(testStr.length() == 7ULL);
}

#include <catch2/test_epilog.h>
