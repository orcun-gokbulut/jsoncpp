
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

#include "JsonTokenizer.h"

#include <string.h>

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

bool JsonTokenizer::Tokenize(const char* jsonText)
{
    JsonToken token;

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

    Current = (Tokens.size() != 0 ? &Tokens[0] : nullptr);

    return true;
}

const JsonToken* JsonTokenizer::RequestToken()
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

void JsonTokenizer::DeferToken()
{
    Current--;
}

JsonTokenizer::JsonTokenizer()
{
    Current = nullptr;
}
