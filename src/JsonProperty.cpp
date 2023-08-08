#include "Json.h"
#include "JsonTokenizer.h"

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
