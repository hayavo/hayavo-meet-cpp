#ifndef HAYAVO_MEET_EXCEPTIONS_H
#define HAYAVO_MEET_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace hayavo::meet
{

    class Exception : public std::runtime_error
    {
    public:
        explicit Exception(const std::string &message)
            : std::runtime_error(message)
        {
        }
    };

    class AuthenticationError : public Exception
    {
    public:
        explicit AuthenticationError(const std::string &message)
            : Exception(message)
        {
        }
    };

    class APIRequestError : public Exception
    {
    public:
        explicit APIRequestError(const std::string &message)
            : Exception(message)
        {
        }
    };

    class ValidationError : public Exception
    {
    public:
        explicit ValidationError(const std::string &message)
            : Exception(message)
        {
        }
    };
}

#endif