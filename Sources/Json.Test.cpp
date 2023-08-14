#include "Json.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Json")
{
    SECTION("Json::Json()")
    {
        Json a;
        REQUIRE(a.GetType() == JsonType::Undefined);
        REQUIRE(a.IsUndefined());
    }

    SECTION("Json::Json(JsonType type)")
    {
        Json a(JsonType::Null);
        REQUIRE(a.GetType() == JsonType::Null);
        REQUIRE(a.IsNull());
    }

    SECTION("Json::Json(const char* value)")
    {
        Json a("TestString");
        REQUIRE(a.GetType() == JsonType::String);
        REQUIRE_NOTHROW(a.GetString() == "TestString");
    }

    SECTION("Json::Json(const std::string& value)")
    {
        Json a("TestString");
        REQUIRE(a.GetType() == JsonType::String);
        REQUIRE_NOTHROW(a.GetString() == "TestString");
    }

    SECTION("Json::Json(bool value)")
    {
        Json a(true);
        REQUIRE(a.GetType() == JsonType::Boolean);
        REQUIRE_NOTHROW(a.GetBoolean() == true);

        Json b(false);
        REQUIRE(a.GetType() == JsonType::Boolean);
        REQUIRE_NOTHROW(a.GetBoolean() == false);
    }

    SECTION("Json::Json(const std::string& value)")
    {
        Json a(1234.0f);
        REQUIRE(a.GetType() == JsonType::Numeric);
        REQUIRE_NOTHROW(a.GetNumeric() == 1234);
    }

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

    SECTION("Json::SetNumeric")
    {
        Json a;
        a.SetNumeric(1234);
        REQUIRE(a.GetType() == JsonType::Numeric);
        REQUIRE_NOTHROW(a.GetNumeric() == 1234);
    }

    SECTION("Json::GetNumeric")
    {
        Json a;
        a.SetNumeric(1234);
        REQUIRE_NOTHROW(a.GetNumeric() == 1234);

        a.SetNull();
        REQUIRE_THROWS_AS(a.GetNumeric(), JsonTypeMismatchException);

        Json b;
        REQUIRE_THROWS_AS(a.GetNumeric(), JsonTypeMismatchException);
    }

    SECTION("Json::SetBoolean")
    {
        Json a;
        a.SetBoolean(true);
        REQUIRE(a.GetType() == JsonType::Boolean);
        REQUIRE_NOTHROW(a.GetBoolean() == true);
    }

    SECTION("Json::GetBoolean")
    {
        Json a;
        a.SetBoolean(true);
        REQUIRE_NOTHROW(a.GetBoolean() == 1234);

        a.SetNull();
        REQUIRE_THROWS_AS(a.GetBoolean(), JsonTypeMismatchException);

        Json b;
        REQUIRE_THROWS_AS(a.GetBoolean(), JsonTypeMismatchException);
    }

    SECTION("Json::AddObjectProperty")
    {
        Json a;
        JsonProperty& newProperty = a.AddObjectProperty("TestProperty");
        REQUIRE(a.GetType() == JsonType::Object);
        REQUIRE(newProperty.Name == "TestProperty");
        REQUIRE(newProperty.Value.IsUndefined());

        newProperty.Value.SetString("TestValue");
        REQUIRE_NOTHROW(!a.GetObjectProperties().empty());
        REQUIRE_NOTHROW(a.GetObjectProperties().at(0).Name == "TestProperty");
        REQUIRE_NOTHROW(a.GetObjectProperties().at(0).Value.GetString() == "TestProperty");
    }

    SECTION("Json::AddObjectProperty")
    {
        Json a;
        JsonProperty& firstProperty = a.AddObjectProperty("TestProperty0", 1234.0);
        REQUIRE(a.GetType() == JsonType::Object);
        REQUIRE(firstProperty.Name == "TestProperty0");
        REQUIRE(firstProperty.Value.GetNumeric() == 1234);
        REQUIRE_NOTHROW(!a.GetObjectProperties().empty());
        REQUIRE_NOTHROW(a.GetObjectProperties().at(0).Name == "TestProperty0");
        REQUIRE_NOTHROW(a.GetObjectProperties().at(0).Value.GetNumeric() == 1234);

        JsonProperty& secondProperty = a.AddObjectProperty("TestProperty1", true);
        REQUIRE(secondProperty.Name == "TestProperty1");
        REQUIRE(secondProperty.Value.GetBoolean() == true);
        REQUIRE_NOTHROW(a.GetObjectProperties().size() == 2);
        REQUIRE_NOTHROW(a.GetObjectProperties().at(1).Name == "TestProperty1");
        REQUIRE_NOTHROW(a.GetObjectProperties().at(1).Value.GetBoolean() == true);

        // Overwrite Property
        JsonProperty& overwritenProperty = a.AddObjectProperty("TestProperty0", "Overwriten");
        REQUIRE(overwritenProperty.Name == "TestProperty0");
        REQUIRE(overwritenProperty.Value.GetString() == "Overwriten");
        REQUIRE_NOTHROW(a.GetObjectProperties().size() == 2);
        REQUIRE_NOTHROW(a.GetObjectProperties().at(0).Name == "TestProperty0");
        REQUIRE_NOTHROW(a.GetObjectProperties().at(0).Value.GetString() == "Overwriten");
    }

    SECTION("Json::AddObjectProperties")
    {
        Json a;
        REQUIRE_THROWS_AS(a.GetObjectProperties(), JsonTypeMismatchException);
    }

    SECTION("Json::RemoveObjectProperty")
    {
        Json a;

        REQUIRE_THROWS_AS(a.RemoveObjectProperty("NonObjectJson"), JsonTypeMismatchException);

        a.AddObjectProperty("TestProperty0", "TestValue");
        a.AddObjectProperty("TestProperty1", false);
        a.AddObjectProperty("TestProperty2", 1234.0);
        REQUIRE(a.GetObjectProperties().size() == 3);

        REQUIRE_THROWS_AS(a.RemoveObjectProperty("NonExistingProperty"), JsonIndexOutOfRangeException);
        REQUIRE(a.GetObjectProperties().size() == 3);

        REQUIRE_NOTHROW(a.RemoveObjectProperty("TestProperty0"));
        REQUIRE(a.GetObjectProperties().size() == 2);
        REQUIRE(a.GetObjectProperties().at(0).Name == "TestProperty1");
        REQUIRE(a.GetObjectProperties().at(0).Value.GetBoolean() == false);


        REQUIRE_NOTHROW(a.RemoveObjectProperty("TestProperty2"));
        REQUIRE(a.GetObjectProperties().size() == 1);
        REQUIRE(a.GetObjectProperties().at(0).Name == "TestProperty1");
        REQUIRE(a.GetObjectProperties().at(0).Value.GetBoolean() == false);

        REQUIRE_NOTHROW(a.RemoveObjectProperty("TestProperty1"));
        REQUIRE(a.GetObjectProperties().size() == 0);

        REQUIRE(a.GetType() == JsonType::Object);
    }

    SECTION("Json::GetObjectProperty")
    {
        Json a;

        REQUIRE_THROWS_AS(a.RemoveObjectProperty("NonObjectJson"), JsonTypeMismatchException);

        a.AddObjectProperty("TestProperty0", "TestValue");
        a.AddObjectProperty("TestProperty1", true);
        a.AddObjectProperty("TestProperty2", 1234.0);
        REQUIRE(a.GetObjectProperties().size() == 3);

        REQUIRE_THROWS_AS(a.GetObjectProperty("NonExistingProperty"), JsonIndexOutOfRangeException);

        REQUIRE_NOTHROW(a.GetObjectProperty("TestProperty0"));

        Json& testProperty0 = a.GetObjectProperty("TestProperty0");
        REQUIRE(testProperty0.GetString() == "TestValue");

        Json& testProperty1 = a.GetObjectProperty("TestProperty1");
        REQUIRE(testProperty1.GetBoolean() == true);

        Json& testProperty2 = a.GetObjectProperty("TestProperty2");
        REQUIRE(testProperty2.GetNumeric() == 1234);

        a.RemoveObjectProperty("TestProperty1");
        REQUIRE_THROWS_AS(a.GetObjectProperty("TestProperty1"), JsonIndexOutOfRangeException);
    }


/*
    Json& AppendArrayItem() noexcept;
    void AppendArrayItem(const Json& Value) noexcept;
    Json& InsertArrayItem(size_t index);
    void InsertArrayItem(size_t index, const Json& value);
    void RemoveArrayItem(size_t index);
    const std::vector<Json>& GetArrayItems() const;

    void SetArrayItem(size_t index, const Json& value);
    Json& GetArrayItem(size_t index);
    const Json& GetArrayItem(size_t index) const;
*/

    SECTION("Json::AddArrayItem")
    {
        Json a;
        Json& newItem0 = a.AddArrayItem();
        REQUIRE(a.GetType() == JsonType::Array);
        REQUIRE(newItem0.IsUndefined());
        REQUIRE_NOTHROW(a.GetArrayItems().size() == 1);
        REQUIRE_NOTHROW(a.GetArrayItems().at(0).IsUndefined());

        Json& newItem1 = a.AddArrayItem(Json::Null);
        REQUIRE(newItem1.IsNull());
        REQUIRE_NOTHROW(a.GetArrayItems().size() == 2);
        REQUIRE_NOTHROW(a.GetArrayItems().at(0).IsUndefined());
        REQUIRE_NOTHROW(a.GetArrayItems().at(1).IsNull());

        Json& newItem2 = a.AddArrayItem("TestValue");
        REQUIRE(newItem2.GetString() == "TestValue");
        REQUIRE_NOTHROW(a.GetArrayItems().size() == 3);
        REQUIRE_NOTHROW(a.GetArrayItems().at(0).IsUndefined());
        REQUIRE_NOTHROW(a.GetArrayItems().at(1).IsNull());
        REQUIRE_NOTHROW(a.GetArrayItems().at(2).GetString() == "TestValue");
    }

    SECTION("Json::GetArrayItems")
    {
        Json a;
        REQUIRE_THROWS_AS(a.GetArrayItems(), JsonTypeMismatchException);
    }

    SECTION("Json::GetArrayItem")
    {
        Json a;
        a.AddArrayItem();
        REQUIRE_NOTHROW(a.GetArrayItem(0).IsUndefined());

        a.AddArrayItem(Json::Null);
        REQUIRE_NOTHROW(a.GetArrayItem(1).IsNull());

        a.AddArrayItem("TestValue");
        REQUIRE_NOTHROW(a.GetArrayItem(2).GetString() == "TestValue");

        a.AddArrayItem(1234.0);
        REQUIRE_NOTHROW(a.GetArrayItem(3).GetNumeric() == 1234);

        a.AddArrayItem(true);
        REQUIRE_NOTHROW(a.GetArrayItem(4).GetBoolean() == true);

        a.AddArrayItem(false);
        REQUIRE_NOTHROW(a.GetArrayItem(5).GetBoolean() == false);
    }

    SECTION("Json::InsertArrayItem")
    {
        Json a;
        Json& newItem = a.InsertArrayItem(0);
        REQUIRE(a.GetType() == JsonType::Array);
        REQUIRE(newItem.IsUndefined());
        REQUIRE(a.GetArrayItems().size() == 1);
        REQUIRE(a.GetArrayItem(0).IsUndefined());

        a.InsertArrayItem(0, true);
        REQUIRE(newItem.GetArrayItems().size() == 2);
        REQUIRE(a.GetArrayItem(0).GetBoolean() == true);

        a.InsertArrayItem(1, false);
        REQUIRE(a.GetArrayItems().size() == 3);
        REQUIRE(a.GetArrayItem(1).GetBoolean() == false);

        a.InsertArrayItem(2, "TestValue");
        REQUIRE(a.GetArrayItems().size() == 4);
        REQUIRE(a.GetArrayItem(2).GetString() == "TestValue");

        REQUIRE_NOTHROW(a.InsertArrayItem(4, "TestValue"));
        REQUIRE(a.GetArrayItems().size() == 5);
        REQUIRE(a.GetArrayItem(2).IsNull());

        REQUIRE_THROWS_AS(a.InsertArrayItem(7, Json::Null), JsonIndexOutOfRangeException);
    }

    SECTION("Json::RemoveArrayItem")
    {
        Json a;
        REQUIRE_THROWS_AS(a.RemoveArrayItem(0), JsonTypeMismatchException);

        a.AddArrayItem(1.0);
        REQUIRE_NOTHROW(a.RemoveArrayItem(0));
        REQUIRE(a.GetArrayItems().empty());

        REQUIRE_THROWS_AS(a.RemoveArrayItem(0), JsonIndexOutOfRangeException);

        a.AddArrayItem(2.0);
        a.AddArrayItem(3.0);
        REQUIRE_NOTHROW(a.RemoveArrayItem(1));
        REQUIRE(a.GetArrayItems().size() == 1);
        REQUIRE(a.GetArrayItem(0).GetNumeric() == 2.0);

        REQUIRE_THROWS_AS(a.RemoveArrayItem(1), JsonIndexOutOfRangeException);
    }
}
