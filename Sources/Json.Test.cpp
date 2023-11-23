/****************************************************************
 JsonCpp - Json.Test.cpp
 ----------------------------------------------------------------
 Copyright (c) 2022, Yiğit Orçun GÖKBULUT. All rights Reserved.

 Use of this source code is governed by a BSD-style
 license that can be found in the LICENSE file or at
 https://developers.google.com/open-source/licenses/bsd

 Copyright Owner Information:
  Name: Yiğit Orçun GÖKBULUT
  Contact: orcun.gokbulut@gmail.com
  Github: https://www.github.com/orcun-gokbulut
****************************************************************/

#include "Json.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("Json::Json()")
{
    Json a;
    REQUIRE(a.GetType() == JsonType::Undefined);
    REQUIRE(a.IsUndefined());
}

TEST_CASE("Json::Json(JsonType type)")
{
    Json a(JsonType::Null);
    REQUIRE(a.GetType() == JsonType::Null);
    REQUIRE(a.IsNull());
}

TEST_CASE("Json::Json(const char* value)")
{
    Json a("TestString");
    REQUIRE(a.GetType() == JsonType::String);
    REQUIRE_NOTHROW(a.GetString() == "TestString");
}

TEST_CASE("Json::Json(const std::string& value)")
{
    Json a(std::string("TestString"));
    REQUIRE(a.GetType() == JsonType::String);
    REQUIRE_NOTHROW(a.GetString() == "TestString");
}

TEST_CASE("Json::Json(bool value)")
{
    Json a(true);
    REQUIRE(a.GetType() == JsonType::Boolean);
    REQUIRE_NOTHROW(a.GetBoolean() == true);

    Json b(false);
    REQUIRE(a.GetType() == JsonType::Boolean);
    REQUIRE_NOTHROW(a.GetBoolean() == false);
}

TEST_CASE("Json::Json(doubles value)")
{
    Json a(1234.0f);
    REQUIRE(a.GetType() == JsonType::Numeric);
    REQUIRE_NOTHROW(a.GetNumeric() == 1234);
}

TEST_CASE("Json::SetString")
{
    Json a;
    a.SetString("TestString");
    REQUIRE(a.GetType() == JsonType::String);
    REQUIRE(a.GetString() == "TestString");
}

TEST_CASE("Json::GetString")
{
    Json a;
    a.SetString("TestString");
    REQUIRE_NOTHROW(a.GetString() == "TestString");

    a.SetNull();
    REQUIRE_THROWS_AS(a.GetString(), JsonTypeMismatchException);

    Json b;
    REQUIRE_THROWS_AS(a.GetString(), JsonTypeMismatchException);
}

TEST_CASE("Json::SetNumeric")
{
    Json a;
    a.SetNumeric(1234);
    REQUIRE(a.GetType() == JsonType::Numeric);
    REQUIRE_NOTHROW(a.GetNumeric() == 1234);
}

TEST_CASE("Json::GetNumeric")
{
    Json a;
    a.SetNumeric(1234);
    REQUIRE_NOTHROW(a.GetNumeric() == 1234);

    a.SetNull();
    REQUIRE_THROWS_AS(a.GetNumeric(), JsonTypeMismatchException);

    Json b;
    REQUIRE_THROWS_AS(a.GetNumeric(), JsonTypeMismatchException);
}

TEST_CASE("Json::SetBoolean")
{
    Json a;
    a.SetBoolean(true);
    REQUIRE(a.GetType() == JsonType::Boolean);
    REQUIRE_NOTHROW(a.GetBoolean() == true);
}

TEST_CASE("Json::GetBoolean")
{
    Json a;
    a.SetBoolean(true);
    REQUIRE_NOTHROW(a.GetBoolean() == 1234);

    a.SetNull();
    REQUIRE_THROWS_AS(a.GetBoolean(), JsonTypeMismatchException);

    Json b;
    REQUIRE_THROWS_AS(a.GetBoolean(), JsonTypeMismatchException);
}

TEST_CASE("Json::AddObjectProperty")
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

TEST_CASE("Json::AddObjectProperty(const std::string&)")
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

TEST_CASE("Json::AddObjectProperties")
{
    Json a;
    REQUIRE_THROWS_AS(a.GetObjectProperties(), JsonTypeMismatchException);
}

TEST_CASE("Json::RemoveObjectProperty")
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

TEST_CASE("Json::SetEmptyObject")
{
    Json a;

    a.SetEmptyObject();
    REQUIRE(a.GetType() == JsonType::Object);
    REQUIRE(a.GetObjectProperties().size() == 0);

    a.AddObjectProperty("A");
    a.AddObjectProperty("B");
    a.AddObjectProperty("C");
    a.AddObjectProperty("D");
    REQUIRE(a.GetObjectProperties().size() == 4);

    a.SetEmptyObject();
    REQUIRE(a.GetType() == JsonType::Object);
    REQUIRE(a.GetObjectProperties().size() == 0);
}

TEST_CASE("Json::GetObjectProperty")
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

TEST_CASE("Json::AddArrayItem")
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

TEST_CASE("Json::GetArrayItems")
{
    Json a;
    REQUIRE_THROWS_AS(a.GetArrayItems(), JsonTypeMismatchException);
}

TEST_CASE("Json::GetArrayItem")
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

TEST_CASE("Json::InsertArrayItem")
{
    Json a;
    Json& newItem = a.InsertArrayItem(0);
    REQUIRE(newItem.IsUndefined());
    REQUIRE(a.GetType() == JsonType::Array);
    REQUIRE(a.GetArrayItems().size() == 1);
    REQUIRE(a.GetArrayItem(0).IsUndefined());

    a.InsertArrayItem(0, true);
    REQUIRE(a.GetArrayItems().size() == 2);
    REQUIRE(a.GetArrayItem(0).GetBoolean() == true);

    a.InsertArrayItem(1, false);
    REQUIRE(a.GetArrayItems().size() == 3);
    REQUIRE(a.GetArrayItem(1).GetBoolean() == false);

    a.InsertArrayItem(2, "TestValue");
    REQUIRE(a.GetArrayItems().size() == 4);
    REQUIRE(a.GetArrayItem(2).GetString() == "TestValue");

    REQUIRE_NOTHROW(a.InsertArrayItem(4, Json::Null));
    REQUIRE(a.GetArrayItems().size() == 5);
    REQUIRE(a.GetArrayItem(4).IsNull());

    REQUIRE_THROWS_AS(a.InsertArrayItem(7, Json::Null), JsonIndexOutOfRangeException);
}

TEST_CASE("Json::SetEmptyArray")
{
    Json a;

    a.SetEmptyArray();
    REQUIRE(a.GetType() == JsonType::Array);
    REQUIRE(a.GetArrayItems().size() == 0);

    a.AddArrayItem(1.0);
    a.AddArrayItem(2.0);
    a.AddArrayItem(3.0);
    REQUIRE(a.GetArrayItems().size() == 3);

    a.SetEmptyArray();
    REQUIRE(a.GetType() == JsonType::Array);
    REQUIRE(a.GetArrayItems().size() == 0);
}

TEST_CASE("Json::RemoveArrayItem")
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

TEST_CASE("Json::Parse")
{
    Json a;
    REQUIRE_NOTHROW(a.Parse("{}"));
    REQUIRE(a.GetType() == JsonType::Object);

    const char* jsonText = R"(
        {
            "Null Property": null,
            "String Property": "StringValue",
            "Numeric Property": 1234,
            "Boolean Property True": true,
            "Boolean Property False": false,
            "Empty Object Property":
            {

            },
            "Object Property":
            {
                "Sub Null Property": null,
                "Sub String Property": "StringValue",
                "Sub Numeric Property": 1234,
                "Sub Boolean Property True": true,
                "Sub Boolean Property False": false,
                "Sub Array Property": [],
                "Sub Object Property": {}
            },
            "Empty Array Property":
            [

            ],
            "Array Property":
            [
                null,
                4321,
                true,
                false,
                "TestValue",
                {},
                {
                    "True Property:": true,
                    "False Property:": false
                },
                [],
                [
                    true,
                    false
                ]
            ]
        }
    )";

    REQUIRE_NOTHROW(a.Parse(jsonText));

    REQUIRE(a.GetType() == JsonType::Object);
    REQUIRE(a.GetObjectProperties().size() == 9);
    REQUIRE(a.GetObjectProperty("Null Property").IsNull());
    REQUIRE(a.GetObjectProperty("String Property").GetString() == "StringValue");
    REQUIRE(a.GetObjectProperty("Numeric Property").GetNumeric() == 1234);
    REQUIRE(a.GetObjectProperty("Boolean Property True").GetBoolean() == true);
    REQUIRE(a.GetObjectProperty("Boolean Property False").GetBoolean() == false);

    REQUIRE(a.GetObjectProperty("Empty Object Property").GetType() == JsonType::Object);
    REQUIRE(a.GetObjectProperty("Empty Object Property").GetObjectProperties().size() == 0);

    REQUIRE(a.GetObjectProperty("Object Property").GetType() == JsonType::Object);
    REQUIRE(a.GetObjectProperty("Object Property").GetObjectProperties().size() == 7);
    REQUIRE(a.GetObjectProperty("Object Property").GetObjectProperty("Sub Null Property").IsNull());
    REQUIRE(a.GetObjectProperty("Object Property").GetObjectProperty("Sub String Property").GetString() == "StringValue");
    REQUIRE(a.GetObjectProperty("Object Property").GetObjectProperty("Sub Numeric Property").GetNumeric() == 1234);
    REQUIRE(a.GetObjectProperty("Object Property").GetObjectProperty("Sub Boolean Property True").GetBoolean() == true);
    REQUIRE(a.GetObjectProperty("Object Property").GetObjectProperty("Sub Boolean Property False").GetBoolean() == false);
    REQUIRE(a.GetObjectProperty("Object Property").GetObjectProperty("Sub Object Property").GetType() == JsonType::Object);
    REQUIRE(a.GetObjectProperty("Object Property").GetObjectProperty("Sub Object Property").GetObjectProperties().size() == 0);
    REQUIRE(a.GetObjectProperty("Object Property").GetObjectProperty("Sub Array Property").GetType() == JsonType::Array);
    REQUIRE(a.GetObjectProperty("Object Property").GetObjectProperty("Sub Array Property").GetArrayItems().size() == 0);

    REQUIRE(a.GetObjectProperty("Empty Array Property").GetType() == JsonType::Array);
    REQUIRE(a.GetObjectProperty("Empty Array Property").GetArrayItems().size() == 0);

    REQUIRE(a.GetObjectProperty("Array Property").GetType() == JsonType::Array);
    REQUIRE(a.GetObjectProperty("Array Property").GetArrayItems().size() == 9);
}

TEST_CASE("Json::Query")
{

    const char* jsonText = R"(
        {
            "Property1": "StringValue",
            "Property2": 123,
            "Property3": true,
            "Property With Spaces":
            [
                1234,
                true,
                "StringItem"
            ],
            "NestedProperty":
            {
                "SubProperty1": "StringValue",
                "SubProperty2": 12345,
                "SubProperty3": false
            }
        }
    )";

    Json a;
    a.Parse(jsonText);
    REQUIRE(a.Query("").GetType() == JsonType::Object);
    REQUIRE(a.Query("").GetObjectProperty("Property1").GetString() == "StringValue");
    REQUIRE(a.Query(" \t \n").GetType() == JsonType::Object);
    REQUIRE(a.Query(" \t \n").GetObjectProperty("Property1").GetString() == "StringValue");
    REQUIRE(a.Query("Property1").GetString() == "StringValue");
    REQUIRE(a.Query(" \nProperty2  ").GetNumeric() == 123);
    REQUIRE(a.Query(" \t \n \t Property3 \t \n").GetBoolean() == true);
    REQUIRE(a.Query("NestedProperty").GetObjectProperties().size() == 3);

    REQUIRE_NOTHROW(a.Query("Property4").IsUndefined());
    REQUIRE_NOTHROW(a.Query("Property1.UnknownProperty").IsUndefined());
    /*REQUIRE_NOTHROW(a.Query("Property1[0]").IsUndefined());
    REQUIRE_NOTHROW(a.Query("Property1[\"UnknownProperty\"]").IsUndefined());*/

    REQUIRE_THROWS_AS(a.Query("["), JsonParsingFailedException);
    REQUIRE_THROWS_AS(a.Query("."), JsonParsingFailedException);
    REQUIRE_THROWS_AS(a.Query("]"), JsonParsingFailedException);
    REQUIRE_THROWS_AS(a.Query("NestedProperty."), JsonParsingFailedException);
    REQUIRE_THROWS_AS(a.Query("NestedProperty. \t"), JsonParsingFailedException);
    REQUIRE_THROWS_AS(a.Query("NestedProperty. \n 22 \t"), JsonParsingFailedException);
    REQUIRE_THROWS_AS(a.Query("NestedProperty["), JsonParsingFailedException);
    REQUIRE_THROWS_AS(a.Query("NestedProperty]"), JsonParsingFailedException);

    REQUIRE_THROWS(a.Query("+"));

    Json& e = a.Query("[\"Property2\"]");
    Json& f = a.Query("NestedProperty \t . \n\t SubProperty1");
}