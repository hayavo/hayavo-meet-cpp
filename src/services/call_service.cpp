#include "hayavo/meet/services/call_service.h"
#include "hayavo/meet/services/staging_service.h"
#include "hayavo/meet/exceptions.h"

namespace hayavo::meet
{

    /*
    |--------------------------------------------------------------------------
    | Constructor
    |--------------------------------------------------------------------------
    */

    CallService::CallService(
        HTTPClient &http,
        const Config &config)
        : http_(http),
          config_(config)
    {
    }

    /*
    |--------------------------------------------------------------------------
    | Start Call
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse CallService::start(const json &payload)
    {
        json request = payload;

        bool audio = true;
        bool video = true;
        bool screen = false;

        if (request.contains("media"))
        {
            json media = request["media"];

            if (media.contains("audio"))
                audio = media["audio"].get<bool>();

            if (media.contains("video"))
                video = media["video"].get<bool>();

            if (media.contains("screen"))
                screen = media["screen"].get<bool>();
        }
        else
        {
            if (request.contains("audio"))
            {
                audio = request["audio"].get<bool>();
                request.erase("audio");
            }

            if (request.contains("video"))
            {
                video = request["video"].get<bool>();
                request.erase("video");
            }

            if (request.contains("screen"))
            {
                screen = request["screen"].get<bool>();
                request.erase("screen");
            }
        }

        if (video && !config_.allowVideo)
        {
            throw ValidationError("Video feature is not allowed.");
        }

        if (audio && !config_.allowAudio)
        {
            throw ValidationError("Audio feature is not allowed.");
        }

        if (screen && !config_.allowScreen)
        {
            throw ValidationError("Screen sharing is not allowed.");
        }

        request["media"] = {
            {"audio", audio},
            {"video", video},
            {"screen", screen}};

        return http_.post(
            "/rtc/call/start",
            request,
            true);
    }

    /*
    |--------------------------------------------------------------------------
    | Host Token
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse CallService::hostToken(
        const std::string &callId,
        const std::string &displayName,
        const std::map<std::string, std::string> &attributes)
    {
        if (callId.empty())
        {
            throw std::invalid_argument(
                "call_id is required");
        }

        json payload;

        payload["call_id"] = callId;

        if (!displayName.empty())
        {
            payload["display_name"] = displayName;
        }

        /*
         * Optional attributes.
         */
        if (!attributes.empty())
        {
            payload["attributes"] = attributes;
        }

        HttpResponse response = http_.post(
            "/rtc/call/token",
            payload,
            true);
        
        StagingConfig stage;

        response.data["data"]["rtc"] = stage.rtc();
        response.data["data"]["signal"] = stage.signal();

        return response;
    }

    /*
    |--------------------------------------------------------------------------
    | Guest Token
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse CallService::guestToken(
        const std::string &callId,
        const std::string &appId,
        const std::string &displayName,
        const std::map<std::string, std::string> &attributes)
    {

        if (callId.empty())
        {
            throw std::invalid_argument(
                "call_id is required");
        }

        if (appId.empty())
        {
            throw std::invalid_argument(
                "app_id is required");
        }

        json payload;

        payload["call_id"] = callId;
        payload["app_id"] = appId;

        /*
         * Optional display name.
         */
        if (!displayName.empty())
        {
            payload["display_name"] = displayName;
        }

        /*
         * Optional attributes.
         */
        if (!attributes.empty())
        {
            payload["attributes"] = attributes;
        }

        HttpResponse response = http_.post(
            "/create/call/guest/token",
            payload,
            false);

        StagingConfig stage;

        response.data["data"]["rtc"] = stage.rtc();
        response.data["data"]["signal"] = stage.signal();

        return response;
    }

    /*
    |--------------------------------------------------------------------------
    | End Call
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse CallService::end(
        const std::string &callId)
    {
        json payload;

        payload["call_id"] = callId;

        return http_.post(
            "/rtc/call/end",
            payload,
            true);
    }

    /*
    |--------------------------------------------------------------------------
    | Mute User
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse CallService::muteUser(
        const std::string &callId,
        const std::string &identity,
        bool mute)
    {
        json payload;

        payload["identity"] = identity;
        payload["mute"] = mute;

        return http_.post(
            "/rtc/call/" + callId + "/mute",
            payload,
            true);
    }

    /*
    |--------------------------------------------------------------------------
    | Disable User Video
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse CallService::videoOffUser(
        const std::string &callId,
        const std::string &identity,
        bool disable)
    {
        json payload;

        payload["identity"] = identity;
        payload["disable"] = disable;

        return http_.post(
            "/rtc/call/" + callId + "/video-off",
            payload,
            true);
    }

    /*
    |--------------------------------------------------------------------------
    | Disable Screen Sharing
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse CallService::screenOffUser(
        const std::string &callId,
        const std::string &identity)
    {
        json payload;

        payload["identity"] = identity;

        return http_.post(
            "/rtc/calls/" + callId + "/screen-off",
            payload,
            true);
    }

    /*
    |--------------------------------------------------------------------------
    | Kick User
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse CallService::kickUser(
        const std::string &callId,
        const std::string &identity)
    {
        json payload;

        payload["identity"] = identity;

        return http_.post(
            "/rtc/call/" + callId + "/kick",
            payload,
            true);
    }

}