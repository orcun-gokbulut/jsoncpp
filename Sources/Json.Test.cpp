#include "Json.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Json")
{
    SECTION("Json::SetString")
    {
        Json a;
        a.SetString("TestString");
        REQUIRE(a.GetType() == JsonType::String);
        REQUIRE(a.GetString() == "TestString");
    }

    SECTION("Json::GetString")
    {
        Json a;
        a.SetString("TestString");
        REQUIRE_NOTHROW(a.GetString() == "TestString");

        a.SetNull();
        REQUIRE_THROWS_AS(a.GetString(), JsonTypeMismatchException);

        Json b;
        REQUIRE_THROWS_AS(a.GetString(), JsonTypeMismatchException);
    }

    SECTION("Json::Json(std::string)")
    {
        Json a("TestString");
        REQUIRE(a.GetType() == JsonType::String);
        REQUIRE(a.GetString() == "TestString");
    }
}
