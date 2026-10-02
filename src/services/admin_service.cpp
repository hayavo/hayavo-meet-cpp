#include "hayavo/meet/services/admin_service.h"

namespace hayavo::meet
{

    /*
    |--------------------------------------------------------------------------
    | Constructor
    |--------------------------------------------------------------------------
    */

    AdminService::AdminService(HTTPClient &http)
        : http_(http)
    {
    }

    /*
    |--------------------------------------------------------------------------
    | Kick Participant
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse AdminService::kick(
        const std::string &room,
        const std::string &identity)
    {
        json payload;

        payload["room_name"] = room;
        payload["identity"] = identity;

        return http_.post(
            "/rtc/av/kick",
            payload,
            true);
    }

    /*
    |--------------------------------------------------------------------------
    | Mute Participant
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse AdminService::mute(
        const std::string &room,
        const std::string &identity)
    {
        json payload;

        payload["room_name"] = room;
        payload["identity"] = identity;

        return http_.post(
            "/rtc/video/mute",
            payload,
            true);
    }

    /*
    |--------------------------------------------------------------------------
    | Room Cleaner
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse AdminService::roomCleaner(
        const std::string &room)
    {
        json payload;

        payload["room_name"] = room;

        return http_.post(
            "/rtc/video/end",
            payload,
            true);
    }

} 