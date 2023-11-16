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

enum class JsonQueryTokenType
{
    Unknown,
    PropertyAccess,
    ArrayOpen,
    ArrayClose,
    Identifier,
    StringLiteral,
    NumericLiteral,
};

class JsonQueryToken
{
    public:
        JsonQueryTokenType Type;
        std::string Parameter;
        size_t LineNumber;
        size_t ColumnNumber;

        void Reset();

        JsonQueryToken();
};

class JsonQueryTokenizer
{
    private:
        std::vector<JsonQueryToken> Tokens;
        JsonQueryToken* Current;


    public:
        void Tokenize(const char* jsonText);

        const JsonQueryToken* RequestToken();
        void DeferToken();

        JsonQueryTokenizer();
};
