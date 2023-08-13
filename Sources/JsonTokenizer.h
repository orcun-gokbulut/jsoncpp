/*******************************************************************************
 JsonCpp - JsonTokenizer.h
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

#define NEXT_TOKEN() \
    parser.RequestToken(); \
    if (token == nullptr) \
        return false;

enum class JsonTokenType
{
    Unknown,
    ObjectOpen,
    ObjectClose,
    ArrayOpen,
    ArrayClose,
    Identifier,
    StringLiteral,
    NumericLiteral,
    Assingment,
    Comma
};

class JsonToken
{
    public:
        JsonTokenType Type;
        std::string Parameter;
        size_t LineNumber;
        size_t ColumnNumber;

        void Reset();

        JsonToken();
};

class JsonTokenizer
{
    private:
        std::vector<JsonToken> Tokens;
        JsonToken* Current;


    public:
        bool Tokenize(const char* jsonText);

        const JsonToken* RequestToken();
        void DeferToken();

        JsonTokenizer();
};
