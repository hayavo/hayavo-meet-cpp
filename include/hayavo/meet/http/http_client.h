#ifndef HAYAVO_MEET_HTTP_HTTP_CLIENT_H
#define HAYAVO_MEET_HTTP_HTTP_CLIENT_H

#include <map>
#include <memory>
#include <string>
#include <curl/curl.h>
#include "../json/single_include/nlohmann/json.hpp"

#include "../config.h"

namespace hayavo::meet
{

    using json = nlohmann::json;

    /*
     * Generic HTTP Response
     */
    struct HttpResponse
    {
        int statusCode = 0;

        std::string body;

        json data;

        std::map<std::string, std::string> headers;

        bool success() const
        {
            return statusCode >= 200 && statusCode < 300;
        }
    };

    /*
     * HTTP Client
     */
    class HTTPClient
    {
    public:
        explicit HTTPClient(const Config &config);

        ~HTTPClient();

        HTTPClient(const HTTPClient &) = delete;
        HTTPClient &operator=(const HTTPClient &) = delete;

        /*
         * HTTP Methods
         */

        HttpResponse get(
            const std::string &endpoint,
            const std::map<std::string, std::string> &query = {},
            bool authRequired = true);

        HttpResponse post(
            const std::string &endpoint,
            const json &payload = json::object(),
            bool authRequired = true);

        HttpResponse put(
            const std::string &endpoint,
            const json &payload = json::object(),
            bool authRequired = true);

        HttpResponse del(
            const std::string &endpoint,
            bool authRequired = true);

        /*
         * Multipart File Upload
         */

        HttpResponse upload(
            const std::string &endpoint,
            const std::string &filePath,
            const std::map<std::string, std::string> &fields = {},
            const std::map<std::string, std::string> &headers = {},
            bool authRequired = false);

    private:
        const Config &config_;

        CURL *curl_ = nullptr;

        /*
         * Generic request implementation
         */

        HttpResponse request(
            const std::string &method,
            const std::string &endpoint,
            const json *payload,
            const std::map<std::string, std::string> &headers,
            bool authRequired);

        /*
         * Build request headers
         */

        std::map<std::string, std::string> buildHeaders(
            bool authRequired,
            bool jsonRequest = true) const;

        /*
         * Build query string
         */

        std::string buildQueryString(
            const std::map<std::string, std::string> &query) const;
    };

} 

#endif