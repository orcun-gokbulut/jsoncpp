/****************************************************************
 JsonCpp - JsonTokenizer.cpp
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

#include "JsonQueryTokenizer.h"

#include "JsonExceptions.h"

#include <string.h>

void JsonQueryToken::Reset()
{
    Type = JsonQueryTokenType::Unknown;
    Parameter = "";
}

JsonQueryToken::JsonQueryToken()
{
    Type = JsonQueryTokenType::Unknown;
    LineNumber = 1;
    ColumnNumber = 1;
}

void JsonQueryTokenizer::Tokenize(const char* jsonText)
{
    JsonQueryToken token;

    bool doubleQuote = false;
    bool numericHasDot = false;

    size_t length = strlen(jsonText);
    for (size_t index = 0; index < length; index++)
    {
        char current = jsonText[index];

        token.ColumnNumber++;
        if (current == '\n')
        {
            token.LineNumber++;
            token.ColumnNumber = 1;
        }

        if (token.Type == JsonQueryTokenType::StringLiteral)
        {
            if ((doubleQuote && current == '\"') || (!doubleQuote && current == '\''))
            {
                token.Type = JsonQueryTokenType::StringLiteral;
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
            switch (current)
            {
                case '\\':
                case '\'':
                {
                    if (token.Type != JsonQueryTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonQueryTokenType::StringLiteral;
                    doubleQuote = false;
                    break;
                }

                case '\"':
                {
                    if (token.Type != JsonQueryTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonQueryTokenType::StringLiteral;
                    doubleQuote = true;
                    break;
                }

                case '.':
                {
                    if (token.Type != JsonQueryTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonQueryTokenType::PropertyAccess;
                    token.Parameter = ".";
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
                    if (token.Type != JsonQueryTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }
                    break;
                }

                case '[':
                {
                    if (token.Type != JsonQueryTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonQueryTokenType::ArrayOpen;
                    token.Parameter = "[";
                    Tokens.push_back(token);
                    token.Reset();
                    break;
                }

                case ']':
                {
                    if (token.Type != JsonQueryTokenType::Unknown)
                    {
                        Tokens.push_back(token);
                        token.Reset();
                    }

                    token.Type = JsonQueryTokenType::ArrayClose;
                    token.Parameter = "]";
                    Tokens.push_back(token);
                    token.Reset();
                    break;
                }

                default:
                {
                    if (token.Type == JsonQueryTokenType::Unknown)
                    {
                        if (isdigit(current) != 0 || current == '-')
                        {
                            token.Type = JsonQueryTokenType::NumericLiteral;
                        }
                        else if (isalpha(current) != 0 || current == '_')
                        {
                            token.Type = JsonQueryTokenType::Identifier;
                        }
                        else
                        {
                            throw JsonParsingFailedException(std::string("Unexpected value character. Character: \'") +
                                current + "', Line: " + std::to_string(token.LineNumber) +
                                ", Column: " + std::to_string(token.ColumnNumber));
                        }
                    }
                    else if (token.Type == JsonQueryTokenType::NumericLiteral)
                    {
                        if ((isdigit(current) == 0 && current != '.') ||
                            (current == '.' && token.Parameter.find('.') != std::string::npos))
                        {
                            throw JsonParsingFailedException(
                                std::string("Unexpected numeric literal character. Character: \'") + current +
                                "', Line: " + std::to_string(token.LineNumber) +
                                ", Column: " + std::to_string(token.ColumnNumber));
                        }
                    }
                    else if (token.Type == JsonQueryTokenType::Identifier)
                    {
                        if (isalnum(current) == 0 && current != '_')
                        {
                            throw JsonParsingFailedException(
                                std::string("Unexpected identifier character. Character: \'") + current + "', Line: " +
                                std::to_string(token.LineNumber) + ", Column: " + std::to_string(token.ColumnNumber));
                        }
                    }

                    token.Parameter += current;
                    break;
                }
            }
        }
    }

    if (token.Type != JsonQueryTokenType::Unknown)
    {
        Tokens.push_back(token);
        token.Reset();
    }

    Current = (Tokens.size() != 0 ? &Tokens[0] : nullptr);
}

const JsonQueryToken* JsonQueryTokenizer::RequestToken()
{
    if (Current == Tokens.end().base())
        return nullptr;

    JsonQueryToken* temp = Current;
    Current++;

    return temp;
}

void JsonQueryTokenizer::DeferToken()
{
    Current--;
}

JsonQueryTokenizer::JsonQueryTokenizer()
{
    Current = nullptr;
}
