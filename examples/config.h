#include <iostream>

#include "hayavo/meet/config.h"
#include "hayavo/meet/http/http_client.h"

using namespace hayavo::meet;

class ClientConfig{

    public:
        ClientConfig()
            : config(init()),
              http(config) {

            };


        Config config;
        HTTPClient http;

        static Config init()
        {
            Config config;

            config.apiKey = "HAYAVO_API_KEY";
            config.appId = "HAYAVO_APP_ID";
            config.appSecret = "HAYAVO_APP_SECRET";
            config.appPlatform = "HAYAVO_APP_PLATFORM";
            config.appIdentifier = "HAYAVO_APP_IDENTIFIER";
            config.appName = "HAYAVO_APP_NAME";

            return config;
        };
};