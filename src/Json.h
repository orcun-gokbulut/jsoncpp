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

enum class JsonValueType
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

class JsonValue
{
    friend class JsonProperty;
    friend class JsonTokenizer;
    private:
        bool ParseInternal(JsonTokenizer& parser);
        std::string ToStringInternal(size_t tabDepth, const JsonToStringOptions& options);

    public:
        JsonValueType Type;
        std::string String;
        double Numeric;
        bool Boolean;
        std::vector<JsonValue*> Array;
        std::vector<JsonProperty*> Properties;

        bool Parse(const char* jsonText);
        std::string ToString(const JsonToStringOptions& options = JsonToStringOptions::Default);

        JsonValue();
        ~JsonValue();
};

class JsonProperty
{
    friend class JsonValue;
    private:
        bool ParseInternal(JsonTokenizer& parser);
        std::string ToStringInternal(size_t depth, const JsonToStringOptions& options);

    public:
        std::string Name;
        JsonValue Value;

        JsonProperty();
        ~JsonProperty();
};
