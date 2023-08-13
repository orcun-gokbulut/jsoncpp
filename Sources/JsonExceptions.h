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
