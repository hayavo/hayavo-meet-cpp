#include "hayavo/meet/services/auth_service.h"

namespace hayavo::meet
{

    /*
    |--------------------------------------------------------------------------
    | Constructor
    |--------------------------------------------------------------------------
    */

    AuthService::AuthService(HTTPClient &http)
        : http_(http)
    {
    }

    /*
    |--------------------------------------------------------------------------
    | Create Authentication Session
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse AuthService::init(
        const std::string &login,
        const std::string &password)
    {
        json payload;

        payload["login"] = login;
        payload["password"] = password;

        return http_.post(
            "/rtm/client/token",
            payload,
            true);
    }

}