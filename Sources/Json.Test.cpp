#include "Json.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Json Class")
{
    SECTION("SetString")
    {
        Json A;
        A.SetString("TestString");

        REQUIRE(A.GetType() == JsonType::String);
        REQUIRE(A.GetString() == "TestString");
    }
}
