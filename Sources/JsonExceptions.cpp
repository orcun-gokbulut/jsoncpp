/*******************************************************************************
 JsonCpp - JsonExceptions.cpp
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
