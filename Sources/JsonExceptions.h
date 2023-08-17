/****************************************************************
 JsonCpp - JsonExceptions.h
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
