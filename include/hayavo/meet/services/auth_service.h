#ifndef HAYAVO_MEET_SERVICES_AUTH_SERVICE_H
#define HAYAVO_MEET_SERVICES_AUTH_SERVICE_H

#include <string>

#include "../http/http_client.h"

namespace hayavo::meet
{

    class AuthService
    {
    public:
        /*
         * Constructor
         */
        explicit AuthService(HTTPClient &http);

        ~AuthService() = default;

        AuthService(const AuthService &) = delete;
        AuthService &operator=(const AuthService &) = delete;

        /*
         * Create authentication session.
         *
         * 
         */
        HttpResponse init(
            const std::string &login,
            const std::string &password);

    private:
        HTTPClient &http_;
    };

} 

#endif 