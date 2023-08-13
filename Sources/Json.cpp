
/*******************************************************************************
 JsonCpp - Json.h
 ------------------------------------------------------------------------------
 Copyright (c) 2022, Yiğit Orçun GÖKBULUT. All rights Reserved.

 This library is free software; you can  redistribute it and/or modify it under
 the terms of the GNU  Lesser General  Public License as  published by the Free
 Software Foundation;  either version 2.1  of the License, or  (at your option)
 any later version.

 This library is  distributed in the hope  that it will be  useful, but WITHOUT
 ANY WARRANTY; without even the implied  warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Lesser General  Public License for more
 details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library;  if not, write  to the Free  Software Foundation,  Inc., 51
 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

 Copyright Owner Information:
  Name: Yiğit Orçun GÖKBULUT
  Contact: orcun.gokbulut@gmail.com
  Github: https://www.github.com/orcun-gokbulut/ZE
*******************************************************************************/

#include "Json.h"

#include "JsonTokenizer.h"

Json Json::Undefined;
Json Json::Null(JsonType::Null);

JsonToStringOptions JsonToStringOptions::Default = {false, "\t"};

void Json::SetType(JsonType type)
{
    if (this->Type == type)
        return;

    this->Type = type;

    this->ValueNumeric = 0.0;
    this->ValueBoolean = false;
    this->ValueString.clear();
    ValueProperties.clear();
    ValueArray.clear();
}

bool Json::ParseInternal(JsonTokenizer& parser)
{
    const JsonToken* token = NEXT_TOKEN();
    switch (token->Type)
    {
        case JsonTokenType::Identifier:
        {
            if (token->Parameter == "true")
            {
                SetBoolean(true);
            }
            else if (token->Parameter == "false")
            {
                SetBoolean(false);
            }
            else if (token->Parameter == "null")
            {
                SetNull();
            }
            else
            {
                throw JsonParsingFailedException(std::string("Unknown value identifier. Token: \'") + token->Parameter +
                    "', Line: " + std::to_string(token->LineNumber) +
                    ", Column: " + std::to_string(token->ColumnNumber));
            }
            break;
        }

        case JsonTokenType::StringLiteral:
        {
            SetString(token->Parameter);
            break;
        }

        case JsonTokenType::NumericLiteral:
        {
            SetNumeric(atof(token->Parameter.c_str()));
            break;
        }

        case JsonTokenType::ObjectOpen:
        {
            this->Type = JsonType::Object;

            token = NEXT_TOKEN();
            if (token->Type == JsonTokenType::ObjectClose)
                return true;
            parser.DeferToken();

            while (true)
            {
                this->ValueProperties.push_back(JsonProperty());
                JsonProperty& newProperty = this->ValueProperties.back();
                if (!newProperty.ParseInternal(parser))
                    return false;

                token = NEXT_TOKEN();
                if (token->Type == JsonTokenType::ObjectClose)
                {
                    break;
                }
                else if (token->Type == JsonTokenType::Comma)
                {
                    continue;
                }
                else
                {
                    throw JsonParsingFailedException(
                        std::string(
                            "Irrelevant token found instead of closing object block '}' or comma ','. Token: \'") +
                        token->Parameter + "', Line: " + std::to_string(token->LineNumber) +
                        ", Column: " + std::to_string(token->ColumnNumber));
                }
            }

            break;
        }

        case JsonTokenType::ArrayOpen:
        {
            this->Type = JsonType::Array;

            token = NEXT_TOKEN();
            if (token->Type == JsonTokenType::ArrayClose)
                return true;
            parser.DeferToken();

            while (true)
            {
                Json newValue;
                this->ValueArray.push_back(Json());
                Json& newItem = this->ValueArray.back();
                if (!newItem.ParseInternal(parser))
                    return false;

                token = NEXT_TOKEN();
                if (token->Type == JsonTokenType::ArrayClose)
                {
                    break;
                }
                else if (token->Type == JsonTokenType::Comma)
                {
                    continue;
                }
                else
                {
                    throw JsonParsingFailedException(
                        std::string(
                            "Irrelevant token found instead of closing array block ']' or comma ','. Token: \'") +
                        token->Parameter + "', Line: " + std::to_string(token->LineNumber) +
                        ", Column: " + std::to_string(token->ColumnNumber));
                }
            }
            break;
        }
    }

    return true;
}

std::string Json::ToStringInternal(size_t tabDepth, const JsonToStringOptions& options)
{
    switch (Type)
    {
        default:
        case JsonType::Undefined:
        {
            return "undefined";
        }

        case JsonType::Object:
        {
            if (ValueProperties.size() == 0)
                return "{}";

            std::string tabSpace;
            for (size_t i = 0; i < tabDepth; i++)
                tabSpace += options.tabString;

            std::string output;
            output += tabSpace + "{\n";

            for (size_t i = 0; i < ValueProperties.size(); i++)
            {
                output += tabSpace + options.tabString + ValueProperties[i].ToStringInternal(tabDepth + 1, options);
                output += (i + 1 != ValueProperties.size() ? ",\n" : "\n");
            }
            output += tabSpace + "}";
            return output;
        }

        case JsonType::Array:
        {
            if (ValueArray.size() == 0)
                return "[]";

            std::string tabSpace;
            for (size_t i = 0; i < tabDepth; i++)
                tabSpace += options.tabString;

            std::string output;
            output += tabSpace + "[\n";

            for (size_t i = 0; i < ValueArray.size(); i++)
            {
                output += tabSpace + options.tabString + ValueArray[i].ToStringInternal(tabDepth + 1, options);
                output += (i + 1 != ValueArray.size() ? ",\n" : "\n");
            }
            output += tabSpace + "]";
            return output;
        }

        case JsonType::String:
        {
            return std::string("\"") + ValueString + "\"";
        }

        case JsonType::Numeric:
        {
            if ((ValueNumeric - (int64_t)ValueNumeric) == 0.0)
                return std::to_string((int64_t)ValueNumeric);
            else
                return std::to_string(ValueNumeric);
        }

        case JsonType::Boolean:
        {
            return (ValueBoolean ? "true" : "false");
        }

        case JsonType::Null:
        {
            return "null";
        }
    }
}

JsonType Json::GetType() const noexcept
{
    return Type;
}

void Json::SetUndefined() noexcept
{
    SetType(JsonType::Undefined);
}

bool Json::IsUndefined() const noexcept
{
    return (GetType() == JsonType::Undefined);
}

void Json::SetNull() noexcept
{
    SetType(JsonType::Null);
}

bool Json::IsNull() const noexcept
{
    return (GetType() == JsonType::Null);
}

void Json::SetString(const std::string& value) noexcept
{
    SetType(JsonType::String);
    ValueString = value;
}

const std::string& Json::GetString() const
{
    if (GetType() != JsonType::String)
        throw JsonTypeMismatchException();

    return ValueString;
}

void Json::SetNumeric(double value) noexcept
{
    SetType(JsonType::Numeric);
    ValueNumeric = value;
}

double Json::GetNumeric() const
{
    if (GetType() != JsonType::Numeric)
        throw JsonTypeMismatchException();

    return ValueNumeric;
}

void Json::SetBoolean(bool value) noexcept
{
    SetType(JsonType::Boolean);
    ValueBoolean = value;
}

bool Json::GetBoolean() const
{
    if (GetType() != JsonType::Boolean)
        throw JsonTypeMismatchException();

    return ValueBoolean;
}

JsonProperty& Json::AddObjectProperty(const std::string& name) noexcept
{
    SetType(JsonType::Object);
    for (auto iter = ValueProperties.begin(); iter != ValueProperties.end(); iter++)
    {
        if (iter->Name != name)
            continue;

        return *iter;
    }

    JsonProperty& newProperty = AddObjectProperty(name);
    newProperty.Name = name;

    return newProperty;
}

void Json::AddObjectProperty(const std::string& name, const Json& value) noexcept
{
    JsonProperty& newProprty = AddObjectProperty(name);
    newProprty.Value = value;
}

const std::vector<JsonProperty>& Json::GetObjectProperties() const
{
    if (GetType() != JsonType::Object)
        throw JsonTypeMismatchException();

    return ValueProperties;
}

void Json::RemoveObjectProperty(const std::string& name)
{
    if (GetType() != JsonType::Object)
        throw JsonTypeMismatchException();

    for (auto iter = ValueProperties.begin(); iter != ValueProperties.end(); iter++)
    {
        if (iter->Name != name)
            continue;

        ValueProperties.erase(iter);
        return;
    }
}

void Json::SetObjectProperty(const std::string& name, const Json& value) noexcept
{
    JsonProperty& property = AddObjectProperty(name);
    property.Name = name;
    property.Value = value;
}

Json& Json::GetObjectProperty(const std::string& name)
{
    if (GetType() != JsonType::Object)
        throw JsonTypeMismatchException();

    for (auto iter = ValueProperties.begin(); iter != ValueProperties.end(); iter++)
    {
        if (iter->Name == name)
            return iter->Value;
    }

    return Json::Undefined;
}

const Json& Json::GetObjectProperty(const std::string& name) const
{
    if (GetType() != JsonType::Object)
        throw JsonTypeMismatchException();

    for (auto iter = ValueProperties.begin(); iter != ValueProperties.end(); iter++)
    {
        if (iter->Name == name)
            return iter->Value;
    }

    return Json::Undefined;
}

Json& Json::AppendArrayItem() noexcept
{
    SetType(JsonType::Array);

    return *ValueArray.insert(ValueArray.end(), Json());
}

void Json::AppendArrayItem(const Json& value) noexcept
{
    SetType(JsonType::Array);

    ValueArray.insert(ValueArray.end(), value);
}

Json& Json::InsertArrayItem(size_t index)
{
    SetType(JsonType::Array);

    if (index >= ValueProperties.size())
        throw JsonIndexOutOfRangeException();

    return *ValueArray.insert(ValueArray.begin() + index, Json());
}

void Json::InsertArrayItem(size_t index, const Json& value)
{
    SetType(JsonType::Array);

    if (index >= ValueProperties.size())
        throw JsonIndexOutOfRangeException();

    this->Type = JsonType::Array;
    ValueArray.insert(ValueArray.begin() + index, value);
}

void Json::RemoveArrayItem(size_t index)
{
    if (GetType() != JsonType::Array)
        throw JsonTypeMismatchException();

    if (index >= ValueArray.size())
        throw JsonIndexOutOfRangeException();

    ValueArray.erase(ValueArray.begin() + index);
}

const std::vector<Json>& Json::GetArrayItems() const
{
    if (GetType() != JsonType::Array)
        throw JsonTypeMismatchException();

    return ValueArray;
}

void Json::SetArrayItem(size_t index, const Json& value)
{
    if (GetType() != JsonType::Array)
        throw JsonTypeMismatchException();

    if (index >= ValueProperties.size())
        throw JsonIndexOutOfRangeException();

    ValueArray[index] = value;
}

Json& Json::GetArrayItem(size_t index)
{
    if (GetType() != JsonType::Array)
        throw JsonTypeMismatchException();

    if (index >= ValueProperties.size())
        throw JsonIndexOutOfRangeException();

    return ValueArray[index];
}

const Json& Json::GetArrayItem(size_t index) const
{
    if (GetType() != JsonType::Array)
        throw JsonTypeMismatchException();

    if (index >= ValueProperties.size())
        return Json::Undefined;

    return ValueArray[index];
}

void Json::Parse(const std::string& jsonText)
{
    JsonTokenizer tokenizer;
    tokenizer.Tokenize(jsonText.c_str());

    ParseInternal(tokenizer);
}

std::string Json::ToString(const JsonToStringOptions& options) noexcept
{
    return ToStringInternal(0, options);
}

Json::Json() noexcept
{
    SetType(JsonType::Undefined);
}

Json::Json(JsonType type) noexcept
{
    SetType(type);
}

Json::Json(const char* value) noexcept
{
    SetString(value);
}

Json::Json(const std::string& value) noexcept
{
    SetString(value);
}

Json::Json(double value) noexcept
{
    SetNumeric(value);
}

Json::Json(bool value) noexcept
{
    SetBoolean(value);
}
