/****************************************************************
 JsonCpp - JsonProperty.cpp
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

#include "Json.h"
#include "JsonTokenizer.h"

bool JsonProperty::ParseInternal(JsonTokenizer& parser)
{
    const JsonToken* token = NEXT_TOKEN();
    if (token->Type != JsonTokenType::StringLiteral &&
        token->Type != JsonTokenType::Identifier &&
        token->Type != JsonTokenType::NumericLiteral)
    {
        throw JsonParsingFailedException(std::string("Unexpected token. Token: \'") + token->Parameter +
            "', Line: " + std::to_string(token->LineNumber) + ", Column: " + std::to_string(token->ColumnNumber));
    }
    this->Name = token->Parameter;

    token = NEXT_TOKEN();
    if (token->Type != JsonTokenType::Assingment)
    {
        throw JsonParsingFailedException(
            std::string("Found irrelevant token instead of assignment operator ':'. Token: \'") + token->Parameter +
            "', Line: " + std::to_string(token->LineNumber) + ", Column: " + std::to_string(token->ColumnNumber));
    }

    if (!Value.ParseInternal(parser))
        return false;

    return true;
}

std::string JsonProperty::ToStringInternal(size_t tabDepth, const JsonToStringOptions& options)
{
    std::string output;

    output = std::string("\"") + Name + "\": ";

    if ((Value.Type == JsonType::Object && Value.ValueProperties.size() != 0) ||
        (Value.Type == JsonType::Array && Value.ValueArray.size() != 0))
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
