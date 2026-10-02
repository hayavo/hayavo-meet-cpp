#include "hayavo/meet/services/room_service.h"
#include "hayavo/meet/services/staging_service.h"

namespace hayavo::meet
{

    RoomService::RoomService(HTTPClient &http)
        : http_(http)
    {
    }

    HttpResponse RoomService::create(
        const std::string &roomName,
        const std::string &mode,
        int maxParticipants)
    {
        if (roomName.empty())
        {
            throw std::invalid_argument("room_name is required");
        }

        if (maxParticipants <= 0)
        {
            throw std::invalid_argument(
                "max_participants must be greater than 0");
        }

        json payload;

        payload["room_name"] = roomName;
        payload["mode"] = mode;
        payload["max_participants"] = maxParticipants;

        if (!payload.contains("room_name"))
        {
            throw std::invalid_argument("room_name is required");
        }

        if (!payload.contains("mode"))
        {
            payload["mode"] = "hayavo_meet_video_party";
        }

        if (!payload.contains("max_participants"))
        {
            payload["max_participants"] = 5;
        }

        return http_.post(
            "/rtc/room/create",
            payload,
            true);
    }

    HttpResponse RoomService::hostToken(
        const std::string &roomName,
        const std::string &displayName,
        const std::map<std::string, std::string> &attributes)
    {
        if (roomName.empty())
        {
            throw std::invalid_argument("room_name is required");
        }
        json payload; 

        payload["room_name"] = roomName;

        if (!displayName.empty())
        {
            payload["display_name"] = displayName;
        }

        if (!attributes.empty())
        {
            payload["attributes"] = attributes;
        }

        HttpResponse response = http_.post(
            "/rtc/room/token/host",
            payload,
            true);

        StagingConfig stage;

        response.data["data"]["rtc"] = stage.rtc();
        response.data["data"]["signal"] = stage.signal();

        return response;
    }

    HttpResponse RoomService::guestToken(
        const std::string &roomName,
        const std::string &appId,
        const std::string &displayName,
        const std::map<std::string, std::string> &attributes)
    {

        if (roomName.empty())
        {
            throw std::invalid_argument("room_name is required");
        }

        if (appId.empty())
        {
            throw std::invalid_argument("app_id is required");
        }

        json payload;

        payload["room_name"] = roomName;
        payload["app_id"] = appId;

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
            "/create/room/token/guest",
            payload,
            false);

        StagingConfig stage;

        response.data["data"]["rtc"] = stage.rtc();
        response.data["data"]["signal"] = stage.signal();

        return response;
    }

} 