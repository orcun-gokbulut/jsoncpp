#include "Json.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Json")
{
    SECTION("Json::SetString")
    {
        Json A;
        A.SetString("TestString");
        REQUIRE(A.GetType() == JsonType::String);
        REQUIRE(A.GetString() == "TestString");
    }

    SECTION("Json::GetString")
    {
        Json A;
        A.SetString("TestString");
        REQUIRE_NOTHROW(A.GetString() == "TestString");

        A.SetNull();
        REQUIRE_THROWS_AS(A.GetString(), JsonTypeMismatchException);

        Json B;
        REQUIRE_THROWS_AS(A.GetString(), JsonTypeMismatchException);
    }

    SECTION("Json::Json(std::string)")
    {
        Json A("TestString");
        REQUIRE(A.GetType() == JsonType::String);
        REQUIRE(A.GetString() == "TestString");
    }
}
