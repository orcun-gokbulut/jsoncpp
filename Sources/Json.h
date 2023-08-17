/****************************************************************
 JsonCpp - Json.h
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

#pragma once

#include "JsonExceptions.h"

#include <string>
#include <vector>

enum class JsonType
{
    Undefined,
    Null,
    String,
    Numeric,
    Boolean,
    Object,
    Array
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

        void SetEmptyObject();
        JsonProperty& AddObjectProperty(const std::string& name) noexcept;
        JsonProperty& AddObjectProperty(const std::string& name, const Json& value) noexcept;
        void RemoveObjectProperty(const std::string& name);
        const std::vector<JsonProperty>& GetObjectProperties() const;

        void SetObjectProperty(const std::string& name, const Json& value) noexcept;
        Json& GetObjectProperty(const std::string& name);
        const Json& GetObjectProperty(const std::string& name) const;

        void SetEmptyArray();
        Json& AddArrayItem() noexcept;
        Json& AddArrayItem(const Json& Value) noexcept;
        Json& InsertArrayItem(size_t index);
        Json& InsertArrayItem(size_t index, const Json& value);
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
        Json(const std::string& value) noexcept;
        Json(const char* value) noexcept;
        Json(double value) noexcept;
        Json(bool value) noexcept;

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
