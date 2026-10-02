#include "hayavo/meet/http/http_client.h"
#include "hayavo/meet/services/staging_service.h"

#include <curl/curl.h>

#include <sstream>
#include <stdexcept>
#include <utility>

namespace hayavo::meet
{

    /*
    |--------------------------------------------------------------------------
    | SDK Constants
    |--------------------------------------------------------------------------
    */

    namespace
    {

        constexpr const char *SDK_NAME = "hayavo-meet-cpp";
        constexpr const char *SDK_VERSION = "1.0.0";

    
        /*
        |--------------------------------------------------------------------------
        | URL Builder
        |--------------------------------------------------------------------------
        |
        | Keeps endpoint concatenation safe when callers provide either:
        |
        |
        */

        std::string buildUrl(
            const std::string &baseUrl,
            const std::string &endpoint)
        {
            if (baseUrl.empty())
            {
                throw std::invalid_argument(
                    "API base URL cannot be empty.");
            }

            if (endpoint.empty())
            {
                return baseUrl;
            }

            std::string base = baseUrl;

            while (!base.empty() && base.back() == '/')
            {
                base.pop_back();
            }

            if (endpoint.front() == '/')
            {
                return base + endpoint;
            }

            return base + "/" + endpoint;
        }

        /*
        |--------------------------------------------------------------------------
        | Curl Global Initializer
        |--------------------------------------------------------------------------
        |
        | Initializes libcurl once for the lifetime of the application.
        |
        */

        class CurlGlobal
        {
        public:
            CurlGlobal()
            {
                CURLcode result =
                    curl_global_init(CURL_GLOBAL_DEFAULT);

                if (result != CURLE_OK)
                {
                    throw std::runtime_error(
                        "Failed to initialize libcurl.");
                }
            }

            ~CurlGlobal()
            {
                curl_global_cleanup();
            }

            CurlGlobal(const CurlGlobal &) = delete;
            CurlGlobal &operator=(const CurlGlobal &) = delete;
        };

        /*
         * One global libcurl initializer.
         */
        static CurlGlobal g_curl;

        /*
         |--------------------------------------------------------------------------
         | Write Callback
         |--------------------------------------------------------------------------
         |
         | Receives response body.
         |
         */

        size_t WriteCallback(
            void *contents,
            size_t size,
            size_t nmemb,
            void *userp)
        {
            const size_t total = size * nmemb;

            auto *buffer =
                static_cast<std::string *>(userp);

            buffer->append(
                static_cast<char *>(contents),
                total);

            return total;
        }

        /*
         |--------------------------------------------------------------------------
         | Header Callback
         |--------------------------------------------------------------------------
         |
         | Receives response headers.
         |
         */

        size_t HeaderCallback(
            char *buffer,
            size_t size,
            size_t nitems,
            void *userdata)
        {
            const size_t total = size * nitems;

            auto *headers =
                static_cast<
                    std::map<std::string, std::string> *>(userdata);

            std::string line(
                buffer,
                total);

            const size_t pos =
                line.find(':');

            /*
             * Ignore status lines and headers without ':'.
             */
            if (pos == std::string::npos)
            {
                return total;
            }

            std::string key =
                line.substr(0, pos);

            std::string value =
                line.substr(pos + 1);

            /*
             * Remove leading whitespace.
             */
            while (!value.empty() &&
                   (value.front() == ' ' ||
                    value.front() == '\t'))
            {
                value.erase(value.begin());
            }

            /*
             * Remove CR/LF.
             */
            while (!value.empty() &&
                   (value.back() == '\r' ||
                    value.back() == '\n'))
            {
                value.pop_back();
            }

            (*headers)[key] = value;

            return total;
        }

    }

    /*
    |--------------------------------------------------------------------------
    | Constructor
    |--------------------------------------------------------------------------
    */

    HTTPClient::HTTPClient(
        const Config &config)
        : config_(config),
          curl_(nullptr)
    {
        curl_ = curl_easy_init();

        if (!curl_)
        {
            throw std::runtime_error(
                "Failed to initialize CURL.");
        }
    }

    /*
    |--------------------------------------------------------------------------
    | Destructor
    |--------------------------------------------------------------------------
    */

    HTTPClient::~HTTPClient()
    {
        if (curl_)
        {
            curl_easy_cleanup(curl_);
            curl_ = nullptr;
        }
    }

    /*
    |--------------------------------------------------------------------------
    | Build Query String
    |--------------------------------------------------------------------------
    */

    std::string HTTPClient::buildQueryString(
        const std::map<std::string, std::string> &query) const
    {
        if (query.empty())
        {
            return "";
        }

        std::ostringstream ss;

        ss << "?";

        bool first = true;

        for (const auto &item : query)
        {
            if (!first)
            {
                ss << "&";
            }

            ss << item.first
               << "="
               << item.second;

            first = false;
        }

        return ss.str();
    }

    /*
    |--------------------------------------------------------------------------
    | HTTP GET
    |--------------------------------------------------------------------------
    */

    HttpResponse HTTPClient::get(
        const std::string &endpoint,
        const std::map<std::string, std::string> &query,
        bool authRequired)
    {
        if (!curl_)
        {
            throw std::runtime_error(
                "CURL is not initialized.");
        }

        HttpResponse response;

        std::string body;

        std::map<std::string, std::string> responseHeaders;

        /*
         * Build API URL from StagingConfig.
         */
        StagingConfig stage;

        std::string url =
            buildUrl(
                stage.staging(),
                endpoint) +
            buildQueryString(query);

        curl_easy_reset(curl_);

        curl_easy_setopt(
            curl_,
            CURLOPT_URL,
            url.c_str());

        curl_easy_setopt(
            curl_,
            CURLOPT_HTTPGET,
            1L);

        curl_easy_setopt(
            curl_,
            CURLOPT_WRITEFUNCTION,
            WriteCallback);

        curl_easy_setopt(
            curl_,
            CURLOPT_WRITEDATA,
            &body);

        curl_easy_setopt(
            curl_,
            CURLOPT_HEADERFUNCTION,
            HeaderCallback);

        curl_easy_setopt(
            curl_,
            CURLOPT_HEADERDATA,
            &responseHeaders);

        curl_easy_setopt(
            curl_,
            CURLOPT_TIMEOUT,
            config_.timeout);

        curl_easy_setopt(
            curl_,
            CURLOPT_FOLLOWLOCATION,
            1L);

        /*
         * Always verify TLS certificates in production.
         */
        curl_easy_setopt(
            curl_,
            CURLOPT_SSL_VERIFYPEER,
            1L);

        curl_easy_setopt(
            curl_,
            CURLOPT_SSL_VERIFYHOST,
            2L);

        /*
         * Build request headers.
         */
        struct curl_slist *headerList = nullptr;

        auto headers =
            buildHeaders(authRequired);

        for (const auto &item : headers)
        {
            std::string header =
                item.first +
                ": " +
                item.second;

            headerList =
                curl_slist_append(
                    headerList,
                    header.c_str());
        }

        curl_easy_setopt(
            curl_,
            CURLOPT_HTTPHEADER,
            headerList);

        /*
         * Execute request.
         */
        CURLcode result =
            curl_easy_perform(curl_);

        if (result != CURLE_OK)
        {
            curl_slist_free_all(headerList);

            throw std::runtime_error(
                curl_easy_strerror(result));
        }

        /*
         * HTTP status.
         */
        long statusCode = 0;

        curl_easy_getinfo(
            curl_,
            CURLINFO_RESPONSE_CODE,
            &statusCode);

        response.statusCode =
            static_cast<int>(statusCode);

        response.body = body;

        response.headers =
            std::move(responseHeaders);

        /*
         * Parse JSON response when possible.
         */
        try
        {
            response.data =
                json::parse(body);
        }
        catch (...)
        {
            /*
             * Response may not be JSON.
             */
        }

        curl_slist_free_all(headerList);

        return response;
    }

    /*
    |--------------------------------------------------------------------------
    | Build Request Headers
    |--------------------------------------------------------------------------
    */

    std::map<std::string, std::string>
    HTTPClient::buildHeaders(
        bool authRequired,
        bool jsonRequest) const
    {
        std::map<std::string, std::string> headers;

        headers["User-Agent"] =
            std::string(SDK_NAME) +
            "/" +
            SDK_VERSION;

        headers["X-SDK-Version"] =
            SDK_VERSION;

        if (jsonRequest)
        {
            headers["Content-Type"] =
                "application/json";

            headers["Accept"] =
                "application/json";
        }

        if (authRequired)
        {
            headers["X-API-Key"] =
                config_.apiKey;

            headers["X-App-ID"] =
                config_.appId;

            headers["X-App-Secret"] =
                config_.appSecret;

            headers["X-App-Platform"] =
                config_.appPlatform;

            headers["X-App-Identifier"] =
                config_.appIdentifier;

            if (!config_.appName.empty())
            {
                headers["X-App-Name"] =
                    config_.appName;
            }

            if (!config_.domain.empty())
            {
                headers["X-Domain"] =
                    config_.domain;
            }
        }

        return headers;
    }

    /*
    |--------------------------------------------------------------------------
    | Generic HTTP Request
    |--------------------------------------------------------------------------
    |
    | Used internally by POST, PUT and DELETE.
    |
    */

    HttpResponse HTTPClient::request(
        const std::string &method,
        const std::string &endpoint,
        const json *payload,
        const std::map<std::string, std::string> &extraHeaders,
        bool authRequired)
    {
        if (!curl_)
        {
            throw std::runtime_error(
                "CURL is not initialized.");
        }

        HttpResponse response;

        std::string body;

        std::map<std::string, std::string> responseHeaders;

        /*
         * Build API URL from StagingConfig.
         */
        StagingConfig stage;

        std::string url =
            buildUrl(
                stage.staging(),
                endpoint);

        curl_easy_reset(curl_);

        curl_easy_setopt(
            curl_,
            CURLOPT_URL,
            url.c_str());

        curl_easy_setopt(
            curl_,
            CURLOPT_WRITEFUNCTION,
            WriteCallback);

        curl_easy_setopt(
            curl_,
            CURLOPT_WRITEDATA,
            &body);

        curl_easy_setopt(
            curl_,
            CURLOPT_HEADERFUNCTION,
            HeaderCallback);

        curl_easy_setopt(
            curl_,
            CURLOPT_HEADERDATA,
            &responseHeaders);

        curl_easy_setopt(
            curl_,
            CURLOPT_TIMEOUT,
            config_.timeout);

        curl_easy_setopt(
            curl_,
            CURLOPT_FOLLOWLOCATION,
            1L);

        curl_easy_setopt(
            curl_,
            CURLOPT_SSL_VERIFYPEER,
            1L);

        curl_easy_setopt(
            curl_,
            CURLOPT_SSL_VERIFYHOST,
            2L);

        /*
        |--------------------------------------------------------------------------
        | HTTP Method
        |--------------------------------------------------------------------------
        */

        if (method == "POST")
        {
            curl_easy_setopt(
                curl_,
                CURLOPT_POST,
                1L);
        }
        else if (method == "PUT")
        {
            curl_easy_setopt(
                curl_,
                CURLOPT_CUSTOMREQUEST,
                "PUT");
        }
        else if (method == "DELETE")
        {
            curl_easy_setopt(
                curl_,
                CURLOPT_CUSTOMREQUEST,
                "DELETE");
        }
        else
        {
            curl_easy_setopt(
                curl_,
                CURLOPT_CUSTOMREQUEST,
                method.c_str());
        }

        /*
        |--------------------------------------------------------------------------
        | JSON Payload
        |--------------------------------------------------------------------------
        */

        std::string jsonBody;

        if (payload != nullptr)
        {
            jsonBody = payload->dump();

            curl_easy_setopt(
                curl_,
                CURLOPT_POSTFIELDS,
                jsonBody.c_str());

            curl_easy_setopt(
                curl_,
                CURLOPT_POSTFIELDSIZE,
                static_cast<long>(
                    jsonBody.size()));
        }

        /*
        |--------------------------------------------------------------------------
        | Request Headers
        |--------------------------------------------------------------------------
        */

        struct curl_slist *headerList = nullptr;

        auto headers =
            buildHeaders(
                authRequired,
                true);

        for (const auto &item : extraHeaders)
        {
            headers[item.first] =
                item.second;
        }

        for (const auto &item : headers)
        {
            std::string header =
                item.first +
                ": " +
                item.second;

            headerList =
                curl_slist_append(
                    headerList,
                    header.c_str());
        }

        curl_easy_setopt(
            curl_,
            CURLOPT_HTTPHEADER,
            headerList);

        /*
        |--------------------------------------------------------------------------
        | Execute Request
        |--------------------------------------------------------------------------
        */

        CURLcode result =
            curl_easy_perform(curl_);

        if (result != CURLE_OK)
        {
            curl_slist_free_all(headerList);

            throw std::runtime_error(
                curl_easy_strerror(result));
        }

        /*
        |--------------------------------------------------------------------------
        | HTTP Status Code
        |--------------------------------------------------------------------------
        */

        long statusCode = 0;

        curl_easy_getinfo(
            curl_,
            CURLINFO_RESPONSE_CODE,
            &statusCode);

        response.statusCode =
            static_cast<int>(statusCode);

        response.body = body;

        response.headers =
            std::move(responseHeaders);

        /*
        |--------------------------------------------------------------------------
        | Parse JSON
        |--------------------------------------------------------------------------
        */

        try
        {
            response.data =
                json::parse(body);
        }
        catch (...)
        {
            /*
             * Ignore non-JSON responses.
             */
        }

        /*
        |--------------------------------------------------------------------------
        | Cleanup
        |--------------------------------------------------------------------------
        */

        curl_slist_free_all(headerList);

        return response;
    }

    /*
    |--------------------------------------------------------------------------
    | HTTP POST
    |--------------------------------------------------------------------------
    */

    HttpResponse HTTPClient::post(
        const std::string &endpoint,
        const json &payload,
        bool authRequired)
    {
        return request(
            "POST",
            endpoint,
            &payload,
            {},
            authRequired);
    }

    /*
    |--------------------------------------------------------------------------
    | HTTP PUT
    |--------------------------------------------------------------------------
    */

    HttpResponse HTTPClient::put(
        const std::string &endpoint,
        const json &payload,
        bool authRequired)
    {
        return request(
            "PUT",
            endpoint,
            &payload,
            {},
            authRequired);
    }

    /*
    |--------------------------------------------------------------------------
    | HTTP DELETE
    |--------------------------------------------------------------------------
    */

    HttpResponse HTTPClient::del(
        const std::string &endpoint,
        bool authRequired)
    {
        return request(
            "DELETE",
            endpoint,
            nullptr,
            {},
            authRequired);
    }

    /*
    |--------------------------------------------------------------------------
    | Multipart File Upload
    |--------------------------------------------------------------------------
    |
    | Uploads a file using multipart/form-data.
    |
    */

    HttpResponse HTTPClient::upload(
        const std::string &endpoint,
        const std::string &filePath,
        const std::map<std::string, std::string> &fields,
        const std::map<std::string, std::string> &extraHeaders,
        bool authRequired)
    {
        if (!curl_)
        {
            throw std::runtime_error(
                "CURL is not initialized.");
        }

        HttpResponse response;

        std::string body;

        std::map<std::string, std::string> responseHeaders;

        /*
         * Build API URL from StagingConfig.
         */
        StagingConfig stage;

        std::string url =
            buildUrl(
                stage.staging(),
                endpoint);

        curl_easy_reset(curl_);

        curl_easy_setopt(
            curl_,
            CURLOPT_URL,
            url.c_str());

        curl_easy_setopt(
            curl_,
            CURLOPT_WRITEFUNCTION,
            WriteCallback);

        curl_easy_setopt(
            curl_,
            CURLOPT_WRITEDATA,
            &body);

        curl_easy_setopt(
            curl_,
            CURLOPT_HEADERFUNCTION,
            HeaderCallback);

        curl_easy_setopt(
            curl_,
            CURLOPT_HEADERDATA,
            &responseHeaders);

        curl_easy_setopt(
            curl_,
            CURLOPT_TIMEOUT,
            config_.timeout);

        curl_easy_setopt(
            curl_,
            CURLOPT_SSL_VERIFYPEER,
            1L);

        curl_easy_setopt(
            curl_,
            CURLOPT_SSL_VERIFYHOST,
            2L);

        curl_easy_setopt(
            curl_,
            CURLOPT_FOLLOWLOCATION,
            1L);

        /*
        |--------------------------------------------------------------------------
        | Build Headers
        |--------------------------------------------------------------------------
        */

        struct curl_slist *headerList = nullptr;

        auto headers =
            buildHeaders(
                authRequired,
                false);

        for (const auto &item : extraHeaders)
        {
            headers[item.first] =
                item.second;
        }

        for (const auto &item : headers)
        {
            std::string header =
                item.first +
                ": " +
                item.second;

            headerList =
                curl_slist_append(
                    headerList,
                    header.c_str());
        }

        curl_easy_setopt(
            curl_,
            CURLOPT_HTTPHEADER,
            headerList);

        /*
        |--------------------------------------------------------------------------
        | Multipart Form
        |--------------------------------------------------------------------------
        */

        curl_mime *mime =
            curl_mime_init(curl_);

        if (!mime)
        {
            curl_slist_free_all(headerList);

            throw std::runtime_error(
                "Failed to initialize MIME.");
        }

        /*
        |--------------------------------------------------------------------------
        | File
        |--------------------------------------------------------------------------
        */

        curl_mimepart *part =
            curl_mime_addpart(mime);

        curl_mime_name(
            part,
            "file");

        CURLcode fileResult =
            curl_mime_filedata(
                part,
                filePath.c_str());

        if (fileResult != CURLE_OK)
        {
            curl_mime_free(mime);
            curl_slist_free_all(headerList);

            throw std::runtime_error(
                curl_easy_strerror(fileResult));
        }

        /*
        |--------------------------------------------------------------------------
        | Additional Fields
        |--------------------------------------------------------------------------
        */

        for (const auto &field : fields)
        {
            part =
                curl_mime_addpart(mime);

            curl_mime_name(
                part,
                field.first.c_str());

            curl_mime_data(
                part,
                field.second.c_str(),
                CURL_ZERO_TERMINATED);
        }

        curl_easy_setopt(
            curl_,
            CURLOPT_MIMEPOST,
            mime);

        /*
        |--------------------------------------------------------------------------
        | Execute
        |--------------------------------------------------------------------------
        */

        CURLcode result =
            curl_easy_perform(curl_);

        if (result != CURLE_OK)
        {
            curl_mime_free(mime);
            curl_slist_free_all(headerList);

            throw std::runtime_error(
                curl_easy_strerror(result));
        }

        /*
        |--------------------------------------------------------------------------
        | HTTP Status Code
        |--------------------------------------------------------------------------
        */

        long statusCode = 0;

        curl_easy_getinfo(
            curl_,
            CURLINFO_RESPONSE_CODE,
            &statusCode);

        response.statusCode =
            static_cast<int>(statusCode);

        response.body = body;

        response.headers =
            std::move(responseHeaders);

        /*
        |--------------------------------------------------------------------------
        | Parse JSON
        |--------------------------------------------------------------------------
        */

        try
        {
            response.data =
                json::parse(body);
        }
        catch (...)
        {
            /*
             * Ignore non-JSON responses.
             */
        }

        /*
        |--------------------------------------------------------------------------
        | Cleanup
        |--------------------------------------------------------------------------
        */

        curl_mime_free(mime);

        curl_slist_free_all(headerList);

        return response;
    }

}