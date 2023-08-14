/*******************************************************************************
 JsonCpp - JsonExceptions.h
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

#include <stdexcept>
#include <string>

enum class JsonError
{
    Unknown = 0,
    TypeMismatch,
    IndexOutOfRange,
    ParsingFailed
};

class JsonException : public std::exception
{
    public:
        virtual JsonError GetError() const noexcept = 0;
        virtual const std::string& ToString() const noexcept = 0;

        virtual const char* what() const noexcept override final;
};

class JsonTypeMismatchException : public JsonException
{
    public:
        virtual JsonError GetError() const noexcept override;
        virtual const std::string& ToString() const noexcept override;
};

class JsonIndexOutOfRangeException : public JsonException
{
    public:
        virtual JsonError GetError() const noexcept override;
        virtual const std::string& ToString() const noexcept override;
};

class JsonParsingFailedException : public JsonException
{
    private:
        std::string ErrorText;

    public:
        virtual JsonError GetError() const noexcept override;
        virtual const std::string& ToString() const noexcept override;

        JsonParsingFailedException(std::string errorText);
};
