#include "hayavo/meet/config.h"

#include "hayavo/meet/exceptions.h"

namespace hayavo::meet
{

    Config::Config(
        const std::string &apiKey,
        const std::string &appId,
        const std::string &appSecret,
        const std::string &appPlatform,
        const std::string &appIdentifier)
        : apiKey(apiKey),
          appId(appId),
          appSecret(appSecret),
          appPlatform(appPlatform),
          appIdentifier(appIdentifier)
    {
    }

    /*
    |--------------------------------------------------------------------------
    | Check Configuration
    |--------------------------------------------------------------------------
    */

    bool Config::isValid() const
    {
        return !apiKey.empty() &&
               !appId.empty() &&
               !appSecret.empty() &&
               !appPlatform.empty() &&
               !appIdentifier.empty();
    }

    /*
    |--------------------------------------------------------------------------
    | Validate Configuration
    |--------------------------------------------------------------------------
    */

    void Config::validate() const
    {
        if (apiKey.empty())
        {
            throw ValidationError("API key is required.");
        }

        if (appId.empty())
        {
            throw ValidationError("App ID is required.");
        }

        if (appSecret.empty())
        {
            throw ValidationError("App secret is required.");
        }

        if (appPlatform.empty())
        {
            throw ValidationError("App platform is required.");
        }

        if (appIdentifier.empty())
        {
            throw ValidationError("App identifier is required.");
        }
    }

}