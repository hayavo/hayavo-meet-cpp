#include "hayavo/meet/utils/validator.h"

namespace hayavo::meet
{

    RequestValidator::RequestValidator(const Config &config)
        : config_(config)
    {
    }

    /*
    |--------------------------------------------------------------------------
    | Validate Configuration
    |--------------------------------------------------------------------------
    */

    void RequestValidator::validate() const
    {
        if (config_.apiKey.empty())
        {
            throw ValidationError("API key is required");
        }

        if (config_.appId.empty())
        {
            throw ValidationError("App ID is required");
        }

        if (config_.appSecret.empty())
        {
            throw ValidationError("App secret is required");
        }

        if (config_.appPlatform.empty())
        {
            throw ValidationError("App platform is required");
        }

        if (config_.appIdentifier.empty())
        {
            throw ValidationError("App identifier is required");
        }
    }

    /*
    |--------------------------------------------------------------------------
    | Check Configuration
    |--------------------------------------------------------------------------
    */

    bool RequestValidator::isValid() const
    {
        return !config_.apiKey.empty() &&
               !config_.appId.empty() &&
               !config_.appSecret.empty() &&
               !config_.appPlatform.empty() &&
               !config_.appIdentifier.empty();
    }

    /*
    |--------------------------------------------------------------------------
    | Build Authentication Headers
    |--------------------------------------------------------------------------
    */

    std::map<std::string, std::string>
    RequestValidator::buildHeaders() const
    {
        validate();

        std::map<std::string, std::string> headers;

        headers["X-API-Key"] = config_.apiKey;
        headers["X-App-ID"] = config_.appId;
        headers["X-App-Secret"] = config_.appSecret;
        headers["X-App-Platform"] = config_.appPlatform;
        headers["X-App-Identifier"] = config_.appIdentifier;

        if (!config_.appName.empty())
        {
            headers["X-App-Name"] = config_.appName;
        }

        if (!config_.domain.empty())
        {
            headers["X-Domain"] = config_.domain;
        }

        return headers;
    }

} 