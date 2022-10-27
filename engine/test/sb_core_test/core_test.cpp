#include <sb_core/core.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>
#include <extern_epilog.h>

#include <sb_core/error.h>

using namespace sb;

#define SB_CTF_TEST SB_CTF_ENABLED

TEST_CASE("Feature enabled", "[core]")
{
    STATIC_REQUIRE(sb_ctf_enabled(TEST));
}

#undef SB_CTF_TEST
#define SB_CTF_TEST SB_CTF_DISABLED

TEST_CASE("Compile Time Feature disabled", "[core]")
{
    STATIC_REQUIRE(!sb_ctf_enabled(TEST));
}

TEST_CASE("Check types size", "[core]")
{
    STATIC_REQUIRE(1 == sizeof(b8));
    STATIC_REQUIRE(4 == sizeof(b32));
    STATIC_REQUIRE(1 == sizeof(u8));
    STATIC_REQUIRE(1 == sizeof(i8));
    STATIC_REQUIRE(2 == sizeof(u16));
    STATIC_REQUIRE(2 == sizeof(i16));
    STATIC_REQUIRE(4 == sizeof(u32));
    STATIC_REQUIRE(4 == sizeof(i32));
    STATIC_REQUIRE(8 == sizeof(u64));
    STATIC_REQUIRE(8 == sizeof(i64));
    STATIC_REQUIRE(8 == sizeof(f64));
    STATIC_REQUIRE(4 == sizeof(f32));
    STATIC_REQUIRE(8 == sizeof(usize));
    STATIC_REQUIRE(8 == sizeof(uptr));
}

