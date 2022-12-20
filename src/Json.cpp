
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

#define NEXT_TOKEN() parser.RequestToken(); if (token == nullptr) return false;

void JsonToken::Reset()
{
    Type = JsonTokenType::Unknown;
    Parameter = "";
}

JsonToken::JsonToken()
{
    Type = JsonTokenType::Unknown;
    LineNumber = 1;
    ColumnNumber = 1;
}

bool JsonParser::Tokenize(const std::string& jsonText)
{
    JsonToken token;

    bool doubleQuote = false;
    bool numericHasDot = false;

    size_t length = jsonText.length();
    for (size_t index = 0; index < length; index++)
    {
        char current = jsonText[index];

        token.ColumnNumber++;
        if (current == '\n')
        {
            token.LineNumber++;
            token.ColumnNumber = 1;
        }

        if (token.Type == JsonTokenType::StringLiteral)
        {
            if ((doubleQuote && current == '\"') || (!doubleQuote && current == '\''))
            {
                token.Type = JsonTokenType::StringLiteral;
                Tokens.push_back(token);
                token.Reset();
            }
            else
            {
                token.Parameter += current;
            }
        }
        else
        {
            switch(current)
            {
                case '\\':
                case '\'':
                {
                    if (token.Type != JsonTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonTokenType::StringLiteral;
                    doubleQuote = false;
                    break;
                }

                case '\"':
                {
                    if (token.Type != JsonTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonTokenType::StringLiteral;
                    doubleQuote = true;
                    break;
                }

                case ',':
                {
                    if (token.Type != JsonTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonTokenType::Comma;
                    token.Parameter =  ",";
                    Tokens.push_back(token);
                    token.Reset();
                    break;
                }

                case ' ':
                case '\t':
                case '\n':
                case '\r':
                case '\f':
                {
                    if (token.Type != JsonTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }
                    break;
                }

                case ':':
                {
                    if (token.Type != JsonTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonTokenType::Assingment;
                    token.Parameter =  ":";
                    Tokens.push_back(token);
                    token.Reset();
                    break;
                }

                case '{':
                {
                    if (token.Type != JsonTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonTokenType::ObjectOpen;
                    token.Parameter =  "{";
                    Tokens.push_back(token);
                    token.Reset();
                    break;
                }

                case '}':
                {
                    if (token.Type != JsonTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonTokenType::ObjectClose;
                    token.Parameter =  "}";
                    Tokens.push_back(token);
                    token.Reset();
                    break;
                }

                case '[':
                {
                    if (token.Type != JsonTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonTokenType::ArrayOpen;
                    token.Parameter =  "[";
                    Tokens.push_back(token);
                    token.Reset();
                    break;
                }

                case ']':
                {
                    if (token.Type != JsonTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonTokenType::ArrayClose;
                    token.Parameter =  "]";
                    Tokens.push_back(token);
                    token.Reset();
                    break;
                }

                default:
                {
                    if (token.Type == JsonTokenType::Unknown)
                    {
                        if (isdigit(current) || current == '-')
                        {
                            token.Type = JsonTokenType::NumericLiteral;
                        }
                        else if (isalpha(current) || current == '_')
                        {
                            token.Type = JsonTokenType::Identifier;
                        }
                        else
                        {
                            fprintf(stderr, "Error: Unexpected value character '%c' at line %zu column %zu.\n", current, token.LineNumber, token.ColumnNumber);
                            return false;
                        }
                    }
                    else if (token.Type == JsonTokenType::NumericLiteral)
                    {
                        if ((!isdigit(current) && current != '.') || (current == '.' && token.Parameter.find('.') != std::string::npos))
                        {
                            fprintf(stderr, "Error: Unexpected numeric literal character '%c' at line %zu column %zu.\n", current, token.LineNumber, token.ColumnNumber);
                            return false;
                        }
                    }
                    else if (token.Type == JsonTokenType::Identifier)
                    {
                        if (!isalnum(current) && current != '_')
                        {
                            fprintf(stderr, "Error: Unexpected identifier character '%c' at line %zu column %zu.\n", current, token.LineNumber, token.ColumnNumber);
                            return false;
                        }
                    }

                    token.Parameter += current;
                    break;
                }
            }
        }
    }

    return true;
}

const JsonToken* JsonParser::RequestToken()
{
    if (Current == Tokens.end().base())
    {
        fprintf(stderr, "Error: End of file reached unexpectedly.\n");
        return nullptr;
    }

    JsonToken* temp = Current;
    Current++;

    return temp;
}

void JsonParser::DeferToken()
{
    Current--;
}

bool JsonParser::Parse(const std::string& jsonText, JsonValue& output)
{
    if (!Tokenize(jsonText))
        return false;

    if (Tokens.size() > 0)
        Current = &Tokens[0];

    output.Parse(*this);

    return true;
}

JsonParser::JsonParser()
{
    Current = nullptr;
}

bool JsonValue::Parse(JsonParser& parser)
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

                if (!newProperty->Parse(parser))
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

                if (!newValue->Parse(parser))
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

std::string JsonValue::ToStdString(size_t tabDepth, const char* tabString)
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
                tabSpace += tabString;

            std::string output;
            output += tabSpace + "{\n";

            for (size_t i = 0; i < Properties.size(); i++)
            {
                output += tabSpace + tabString + Properties[i]->ToStdString(tabDepth + 1, tabString);
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
                tabSpace += tabString;

            std::string output;
            output += tabSpace + "[\n";

            for (size_t i = 0; i < Array.size(); i++)
            {
                output += tabSpace + tabString + Array[i]->ToStdString(tabDepth + 1, tabString);
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

bool JsonProperty::Parse(JsonParser& parser)
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

    if (!Value.Parse(parser))
        return false;

    return true;
}

std::string JsonProperty::ToStdString(size_t tabDepth, const char* tabString)
{
    std::string output;

    output = std::string("\"") + Name + "\": ";

    if ((Value.Type == JsonValueType::Object && Value.Properties.size() != 0) ||
        (Value.Type == JsonValueType::Array && Value.Array.size() != 0))
    {
        output += "\n";
    }
    output += Value.ToStdString(tabDepth, tabString);

    return output;
}

JsonProperty::JsonProperty()
{

}

JsonProperty::~JsonProperty()
{

}
