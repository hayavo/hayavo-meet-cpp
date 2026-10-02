#ifndef HAYAVO_MEET_UTILS_VALIDATOR_H
#define HAYAVO_MEET_UTILS_VALIDATOR_H

#include <map>
#include <string>

#include "../config.h"
#include "../exceptions.h"

namespace hayavo::meet
{

    class RequestValidator
    {
    public:
        explicit RequestValidator(const Config &config);

        /*
         * Validate SDK configuration.
         *
         * Throws ValidationError if any required field
         * is missing.
         */
        void validate() const;

        /*
         * Returns true if the configuration is valid.
         */
        bool isValid() const;

        /*
         * Build authentication headers.
         */
        std::map<std::string, std::string> buildHeaders() const;

    private:
        const Config &config_;
    };

    // class ValidationError : public std::runtime_error
    // {
    // public:
    //     explicit ValidationError(const std::string &message)
    //         : std::runtime_error(message)
    //     {
    //     }
    // };
}

#endif