#ifndef HAYAVO_MEET_SERVICES_CALL_SERVICE_H
#define HAYAVO_MEET_SERVICES_CALL_SERVICE_H
#include <map>
#include <string>

#include "../config.h"
#include "../http/http_client.h"
#include "../json/single_include/nlohmann/json.hpp"

namespace hayavo::meet
{

    /*
    |--------------------------------------------------------------------------
    | Call Service
    |--------------------------------------------------------------------------
    |
    | Provides APIs for creating, managing and controlling calls.
    |
    */

    class CallService
    {
    public:
        
        CallService(
            HTTPClient &http,
            const Config &config);

        ~CallService() = default;

        CallService(const CallService &) = delete;
        CallService &operator=(const CallService &) = delete;

        /*
        |--------------------------------------------------------------------------
        | Start Call
        |--------------------------------------------------------------------------
        |
        | 
        |
        */
        HttpResponse start(
            const json &payload);

        /*
        |--------------------------------------------------------------------------
        | Host Token
        |--------------------------------------------------------------------------
        |
        | 
        |
        */
        HttpResponse hostToken(
            const std::string &callId,
            const std::string &displayName = "",
            const std::map<std::string, std::string> &attributes = {});

        /*
        |--------------------------------------------------------------------------
        | Guest Token
        |--------------------------------------------------------------------------
        |
        | 
        |
        */
        HttpResponse guestToken(
            const std::string &callId,
            const std::string &appId,
            const std::string &displayName = "",
            const std::map<std::string, std::string> &attributes = {});

        /*
        |--------------------------------------------------------------------------
        | End Call
        |--------------------------------------------------------------------------
        |
        | 
        |
        */
        HttpResponse end(
            const std::string &callId);

        /*
        |--------------------------------------------------------------------------
        | Mute User
        |--------------------------------------------------------------------------
        |
        | 
        |
        */
        HttpResponse muteUser(
            const std::string &callId,
            const std::string &identity,
            bool mute = true);

        /*
        |--------------------------------------------------------------------------
        | Disable User Video
        |--------------------------------------------------------------------------
        |
        | 
        |
        */
        HttpResponse videoOffUser(
            const std::string &callId,
            const std::string &identity,
            bool disable = true);

        /*
        |--------------------------------------------------------------------------
        | Disable Screen Sharing
        |--------------------------------------------------------------------------
        |
        | 
        |
        */
        HttpResponse screenOffUser(
            const std::string &callId,
            const std::string &identity);

        /*
        |--------------------------------------------------------------------------
        | Kick User
        |--------------------------------------------------------------------------
        |
        | 
        |
        */
        HttpResponse kickUser(
            const std::string &callId,
            const std::string &identity);

    private:
        HTTPClient &http_;

        const Config &config_;
    };

}

#endif