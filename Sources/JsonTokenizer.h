/****************************************************************
 JsonCpp - JsonTokenizer.h
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
