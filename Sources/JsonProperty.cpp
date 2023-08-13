#include "Json.h"
#include "JsonTokenizer.h"

bool JsonProperty::ParseInternal(JsonTokenizer& parser)
{
    const JsonToken* token = NEXT_TOKEN();
    if (token->Type != JsonTokenType::StringLiteral &&
        token->Type != JsonTokenType::Identifier &&
        token->Type != JsonTokenType::NumericLiteral)
    {
        throw JsonParsingFailedException(
            std::string("Unexpected token. Token: \'") + token->Parameter + "', Line: " + std::to_string(token->LineNumber) + ", Column: " + std::to_string(token->ColumnNumber));
    }
    this->Name = token->Parameter;

    token = NEXT_TOKEN();
    if (token->Type != JsonTokenType::Assingment)
    {
        throw JsonParsingFailedException(
            std::string("Found irrelevant token instead of assignment operator ':'. Token: \'") + token->Parameter + "', Line: " + std::to_string(token->LineNumber) + ", Column: " + std::to_string(token->ColumnNumber));
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
