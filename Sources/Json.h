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

#pragma once

#include <string>
#include <vector>

enum class JsonType
{
    Undefined,
    Object,
    Array,
    String,
    Numeric,
    Boolean,
    Null
};

class JsonProperty;
class JsonTokenizer;

class JsonToStringOptions
{
    public:
        bool minmalize;
        const char* tabString;

        static JsonToStringOptions Default;
};

class Json
{
    friend class JsonProperty;
    friend class JsonTokenizer;
    private:
        JsonType Type;
        std::string ValueString;
        double ValueNumeric;
        bool ValueBoolean;
        std::vector<Json> ValueArray;
        std::vector<JsonProperty> ValueProperties;

        void SetType(JsonType Type);

        bool ParseInternal(JsonTokenizer& parser);
        std::string ToStringInternal(size_t tabDepth, const JsonToStringOptions& options);

    public:
        JsonType GetType() const;

        void SetUndefined();
        bool IsUndefined() const;

        void SetNull();
        bool IsNull() const;

        void SetString(const std::string& value);
        const std::string& GetString() const;

        void SetNumeric(double value);
        double GetNumeric() const;

        void SetBoolean(bool value);
        bool GetBoolean() const;

        JsonProperty& AddObjectProperty(const std::string& name);
        const std::vector<JsonProperty>& GetObjectProperties() const;
        void RemoveObjectProperty(const std::string& name);

        void SetObjectProperty(const std::string& name, const Json& value);
        Json& GetObjectProperty(const std::string& name);
        const Json& GetObjectProperty(const std::string& name) const;

        void AppendArrayItem(const Json& Value);
        void InsertArrayItem(size_t index, const Json& value);
        void RemoveArrayItem(size_t index);
        const std::vector<Json>& GetArrayItems() const;

        void SetArrayItem(size_t index, const Json& value);
        Json& GetArrayItem(size_t index);
        const Json& GetArrayItem(size_t index) const;

        bool Parse(const std::string& jsonText);
        std::string ToString(const JsonToStringOptions& options = JsonToStringOptions::Default);

        Json();
        Json(JsonType type);
        Json(const Json& other) = default;
        Json(const std::string& string);
        Json(double numeric);
        Json(bool boolean);

        static Json Undefined;
        static Json Null;
};


class JsonProperty
{
    friend class Json;
    private:
        bool ParseInternal(JsonTokenizer& parser);
        std::string ToStringInternal(size_t depth, const JsonToStringOptions& options);

    public:
        std::string Name;
        Json Value;

        JsonProperty();
        ~JsonProperty();
};
