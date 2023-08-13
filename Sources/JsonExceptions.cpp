#include "JsonExceptions.h"

const char* JsonException::what() const noexcept
{
    return this->ToString().c_str();
}

JsonError JsonTypeMismatchException::GetError() const noexcept
{
    return JsonError::TypeMismatch;
}

const std::string& JsonTypeMismatchException::ToString() const noexcept
{
    static std::string errorText = "Json value type mismatch.";
    return errorText;
}

JsonError JsonIndexOutOfRangeException::GetError() const noexcept
{
    return JsonError::IndexOutOfRange;
}

const std::string& JsonIndexOutOfRangeException::ToString() const noexcept
{
    static std::string errorText = "Index out of range.";
    return errorText;
}

JsonError JsonParsingFailedException::GetError() const noexcept
{
    return JsonError::ParsingFailed;
}

const std::string& JsonParsingFailedException::ToString() const noexcept
{
    return ErrorText;
}

JsonParsingFailedException::JsonParsingFailedException(std::string errorText)
{
    ErrorText = errorText;
}

