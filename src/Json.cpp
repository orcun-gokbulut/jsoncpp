
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

JsonToStringOptions JsonToStringOptions::Default =
{
    false,
    "\t"
};

bool JsonValue::ParseInternal(JsonTokenizer& parser)
{
    const JsonToken* token = NEXT_TOKEN();
    switch(token->Type)
    {
        case JsonTokenType::Identifier:
        {
            if (token->Parameter == "true")
            {
                this->Type = JsonValueType::Boolean;
                this->Boolean = true;
            }
            else if (token->Parameter == "false")
            {
                this->Type = JsonValueType::Boolean;
                this->Boolean = false;
            }
            else if (token->Parameter == "null")
            {
                this->Type = JsonValueType::Null;
            }
            else
            {
                fprintf(stderr, "Error: Unknown value identifier '%s' at line %zu column %zu.\n", token->Parameter.c_str(), token->LineNumber, token->ColumnNumber);
                return false;
            }
            break;
        }

        case JsonTokenType::StringLiteral:
        {
            this->Type = JsonValueType::String;
            this->String = token->Parameter;
            break;
        }

        case JsonTokenType::NumericLiteral:
        {
            this->Type = JsonValueType::Numeric;
            this->Numeric = atof(token->Parameter.c_str());
            break;
        }

        case JsonTokenType::ObjectOpen:
        {
            this->Type = JsonValueType::Object;

            token = NEXT_TOKEN();
            if (token->Type == JsonTokenType::ObjectClose)
                return true;
            parser.DeferToken();

            while (true)
            {
                JsonProperty* newProperty = new JsonProperty();
                this->Properties.push_back(newProperty);

                if (!newProperty->ParseInternal(parser))
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
                    fprintf(stderr, "Error: Found '%s' instead of closing array ']' or comma ',' at line %zu column %zu.\n", token->Parameter.c_str(), token->LineNumber, token->ColumnNumber);
                    return false;
                }
            }

            break;
        }

        case JsonTokenType::ArrayOpen:
        {
            this->Type = JsonValueType::Array;

            token = NEXT_TOKEN();
            if (token->Type == JsonTokenType::ArrayClose)
                return true;
            parser.DeferToken();

            while (true)
            {
                JsonValue* newValue = new JsonValue();
                this->Array.push_back(newValue);

                if (!newValue->ParseInternal(parser))
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
                    fprintf(stderr, "Error: Found '%s' instead of closing array ']' or comma ',' at line %zu column %zu.\n", token->Parameter.c_str(), token->LineNumber, token->ColumnNumber);
                    return false;
                }
            }
            break;
        }
    }

    return true;
}

std::string JsonValue::ToStringInternal(size_t tabDepth, const JsonToStringOptions& options)
{
    switch (Type)
    {
        default:
        case JsonValueType::Undefined:
        {
            return "undefined";
        }

        case JsonValueType::Object:
        {
            if (Properties.size() == 0)
                return "{}";

            std::string tabSpace;
            for (size_t i = 0; i < tabDepth; i++)
                tabSpace += options.tabString;

            std::string output;
            output += tabSpace + "{\n";

            for (size_t i = 0; i < Properties.size(); i++)
            {
                output += tabSpace + options.tabString + Properties[i]->ToStringInternal(tabDepth + 1, options);
                output += (i + 1 != Properties.size() ? ",\n" : "\n");
            }
            output += tabSpace + "}";
            return output;
        }

        case JsonValueType::Array:
        {
            if (Array.size() == 0)
                return "[]";

            std::string tabSpace;
            for (size_t i = 0; i < tabDepth; i++)
                tabSpace += options.tabString;

            std::string output;
            output += tabSpace + "[\n";

            for (size_t i = 0; i < Array.size(); i++)
            {
                output += tabSpace + options.tabString + Array[i]->ToStringInternal(tabDepth + 1, options);
                output += (i + 1 != Array.size() ? ",\n" : "\n");
            }
            output += tabSpace + "]";
            return output;
        }

        case JsonValueType::String:
        {
            return std::string("\"") + String + "\"";
        }

        case JsonValueType::Numeric:
        {
            if ((Numeric - (int64_t)Numeric) == 0.0)
                return std::to_string((int64_t) Numeric);
            else
                return std::to_string(Numeric);
        }

        case JsonValueType::Boolean:
        {
            return (Boolean ? "true" : "false");
        }

        case JsonValueType::Null:
        {
            return "null";
        }
    }
}

bool JsonValue::Parse(const char* jsonText)
{
    JsonTokenizer tokenizer;
    if (!tokenizer.Tokenize(jsonText))
        return false;
    return ParseInternal(tokenizer);
}

std::string JsonValue::ToString(const JsonToStringOptions& options)
{
    return ToStringInternal(0, options);
}

JsonValue::JsonValue()
{
    this->Type = JsonValueType::Undefined;
    this->Numeric = 0.0;
    this->Boolean = false;
}

JsonValue::~JsonValue()
{
    for (size_t i = 0; i < this->Array.size(); i++)
        delete this->Array[i];
    this->Array.clear();

    for (size_t i = 0; i < this->Properties.size(); i++)
        delete this->Properties[i];
    this->Properties.clear();
}

bool JsonProperty::ParseInternal(JsonTokenizer& parser)
{
    const JsonToken* token = NEXT_TOKEN();
    if (token->Type != JsonTokenType::StringLiteral &&
        token->Type != JsonTokenType::Identifier &&
        token->Type != JsonTokenType::NumericLiteral)
    {
        fprintf(stderr, "Error: Unexpected token '%s' at line %zu column %zu.\n", token->Parameter.c_str(), token->LineNumber, token->ColumnNumber);
        return false;
    }
    this->Name = token->Parameter;

    token = NEXT_TOKEN();
    if (token->Type != JsonTokenType::Assingment)
    {
        fprintf(stderr, "Error: Found '%s' instead of assignment operator ':' at line %zu column %zu.\n", token->Parameter.c_str(), token->LineNumber, token->ColumnNumber);
        return false;
    }

    if (!Value.ParseInternal(parser))
        return false;

    return true;
}

std::string JsonProperty::ToStringInternal(size_t tabDepth, const JsonToStringOptions& options)
{
    std::string output;

    output = std::string("\"") + Name + "\": ";

    if ((Value.Type == JsonValueType::Object && Value.Properties.size() != 0) ||
        (Value.Type == JsonValueType::Array && Value.Array.size() != 0))
    {
        output += "\n";
    }
    output += Value.ToStringInternal(tabDepth, options);

    return output;
}

JsonProperty::JsonProperty()
{

}

JsonProperty::~JsonProperty()
{

}
