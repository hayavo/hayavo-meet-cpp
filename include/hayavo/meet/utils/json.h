#ifndef HAYAVO_MEET_UTILS_JSON_H
#define HAYAVO_MEET_UTILS_JSON_H

#include <string>
#include <nlohmann/json.hpp>

namespace hayavo::meet
{

    /*
    |--------------------------------------------------------------------------
    | JSON Type Alias
    |--------------------------------------------------------------------------
    |
    | SDK-wide JSON object.
    |
    */

    using Json = nlohmann::json;

    /*
    |--------------------------------------------------------------------------
    | JSON Utilities
    |--------------------------------------------------------------------------
    */

    class JsonUtils
    {
    public:
        /*
         * Parse JSON string.
         *
         * Returns an empty object if parsing fails.
         */
        static Json parse(const std::string &text)
        {
            try
            {
                return Json::parse(text);
            }
            catch (...)
            {
                return Json::object();
            }
        }

        /*
         * Convert JSON object to string.
         */
        static std::string stringify(const Json &json)
        {
            return json.dump();
        }

        /*
         * Pretty-print JSON.
         */
        static std::string pretty(const Json &json)
        {
            return json.dump(4);
        }

        /*
         * Check whether a string contains valid JSON.
         */
        static bool isValid(const std::string &text)
        {
            try
            {
                Json::parse(text);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        /*
         * Create an empty JSON object.
         */
        static Json object()
        {
            return Json::object();
        }

        /*
         * Create an empty JSON array.
         */
        static Json array()
        {
            return Json::array();
        }
    };

} 

#endif 