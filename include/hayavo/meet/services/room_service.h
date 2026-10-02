#ifndef HAYAVO_MEET_SERVICES_ROOM_SERVICE_H
#define HAYAVO_MEET_SERVICES_ROOM_SERVICE_H
#include <map>
#include <string>

#include "../http/http_client.h"
#include "../json/single_include/nlohmann/json.hpp"

namespace hayavo::meet
{

    /*
    |--------------------------------------------------------------------------
    | Room Service
    |--------------------------------------------------------------------------
    |
    | Provides APIs for creating rooms and generating host/guest tokens.
    |
    */

    class RoomService
    {
    public:
        /*
         * Constructor
         */
        explicit RoomService(HTTPClient &http);

        ~RoomService() = default;

        RoomService(const RoomService &) = delete;
        RoomService &operator=(const RoomService &) = delete;

        
        HttpResponse create(
            const std::string &roomName,
            const std::string &mode = "hayavo_meet_video_party",
            int maxParticipants = 5
        );

        /*
        |--------------------------------------------------------------------------
        | Host Token
        |--------------------------------------------------------------------------
        |
        | 
        |
        */
        HttpResponse hostToken(
            const std::string &roomName,
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
            const std::string &roomName,
            const std::string &appId,
            const std::string &displayName = "",
            const std::map<std::string, std::string> &attributes = {});

    private:
        HTTPClient &http_;
    };

}

#endif 