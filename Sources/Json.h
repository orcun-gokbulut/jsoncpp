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

#include "JsonExceptions.h"

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
        JsonType GetType() const noexcept;

        void SetUndefined() noexcept;
        bool IsUndefined() const noexcept;

        void SetNull() noexcept;
        bool IsNull() const noexcept;

        void SetString(const std::string& value) noexcept;
        const std::string& GetString() const;

        void SetNumeric(double value) noexcept;
        double GetNumeric() const;

        void SetBoolean(bool value) noexcept;
        bool GetBoolean() const;

        JsonProperty& AddObjectProperty(const std::string& name) noexcept;
        void AddObjectProperty(const std::string& name, const Json& value) noexcept;
        void RemoveObjectProperty(const std::string& name);;
        const std::vector<JsonProperty>& GetObjectProperties() const;;

        void SetObjectProperty(const std::string& name, const Json& value) noexcept;
        Json& GetObjectProperty(const std::string& name);
        const Json& GetObjectProperty(const std::string& name) const;

        Json& AppendArrayItem() noexcept;
        void AppendArrayItem(const Json& Value) noexcept;
        Json& InsertArrayItem(size_t index);
        void InsertArrayItem(size_t index, const Json& value);
        void RemoveArrayItem(size_t index);
        const std::vector<Json>& GetArrayItems() const;

        void SetArrayItem(size_t index, const Json& value);
        Json& GetArrayItem(size_t index);
        const Json& GetArrayItem(size_t index) const;

        void Parse(const std::string& jsonText);
        std::string ToString(const JsonToStringOptions& options = JsonToStringOptions::Default) noexcept;

        Json() noexcept;
        Json(JsonType type) noexcept;
        Json(const Json& other) = default;
        Json(const std::string& string) noexcept;
        Json(const char* string) noexcept;
        Json(double numeric) noexcept;
        Json(bool boolean) noexcept;

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
