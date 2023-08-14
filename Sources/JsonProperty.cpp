/*******************************************************************************
 JsonCpp - JsonProperty.cpp
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
  Github: https://www.github.com/orcun-gokbulut
*******************************************************************************/

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
