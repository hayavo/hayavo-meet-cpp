#ifndef HAYAVO_MEET_SERVICES_ADMIN_SERVICE_H
#define HAYAVO_MEET_SERVICES_ADMIN_SERVICE_H

#include <string>

#include "../http/http_client.h"
#include "../json/single_include/nlohmann/json.hpp"

namespace hayavo::meet
{

    /*
    |--------------------------------------------------------------------------
    | Admin Service
    |--------------------------------------------------------------------------
    |
    | Administrative APIs for managing rooms and participants.
    |
    */

    class AdminService
    {
    public:
        /*
        |--------------------------------------------------------------------------
        | Constructor
        |--------------------------------------------------------------------------
        */

        explicit AdminService(HTTPClient &http);

        ~AdminService() = default;

        AdminService(const AdminService &) = delete;
        AdminService &operator=(const AdminService &) = delete;

        /*
        |--------------------------------------------------------------------------
        | Kick Participant
        |--------------------------------------------------------------------------
        |
        | 
        |
        */

        HttpResponse kick(
            const std::string &room,
            const std::string &identity);

        /*
        |--------------------------------------------------------------------------
        | Mute Participant
        |--------------------------------------------------------------------------
        |
        | 
        |
        */

        HttpResponse mute(
            const std::string &room,
            const std::string &identity);

        /*
        |--------------------------------------------------------------------------
        | End / Clean Room
        |--------------------------------------------------------------------------
        |
        | 
        |
        */

        HttpResponse roomCleaner(
            const std::string &room);

    private:
        HTTPClient &http_;
    };

}

#endif