#include <iostream>

#include "hayavo/meet/config.h"
#include "hayavo/meet/http/http_client.h"
#include "hayavo/meet/services/auth_service.h"

using namespace hayavo::meet;

int main()
{
    Config config;

    config.apiKey = "HAYAVO_API_KEY";
    config.appId = "HAYAVO_APP_ID";
    config.appSecret = "HAYAVO_APP_SECRET";
    config.appPlatform = "HAYAVO_APP_PLATFORM";
    config.appIdentifier = "HAYAVO_APP_IDENTIFIER";
    config.appName = "HAYAVO_APP_NAME";

    HTTPClient http(config);

    AuthService auth(http);

    try
    {
        HttpResponse response =
            auth.init(
                "YOUR_REGISTER_EMAIL_ID",
                "PASSWORD");

        std::cout << response.data.dump(4) << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}