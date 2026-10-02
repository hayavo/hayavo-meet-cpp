#ifndef HAYAVO_MEET_CONFIG_H
#define HAYAVO_MEET_CONFIG_H

#include <string>

namespace hayavo::meet
{
    class Config

    {
        public:
            Config() = default;

            Config(
                const std::string &apiKey,
                const std::string &appId,
                const std::string &appSecret,
                const std::string &appPlatform,
                const std::string &appIdentifier);

            ~Config() = default;

            /*
             * Credentials
             */

            std::string apiKey;
            std::string appId;
            std::string appSecret;

            /*
             * Application Information
             */

            std::string appPlatform;
            std::string appIdentifier;

            /*
             * Optional
             */

            std::string appName;
            std::string domain;

            /*
             * Network
             */

            int timeout = 10;

            /*
             * Feature Flags
             */

            bool allowVideo = true;
            bool allowAudio = true;
            bool allowScreen = true;

            /*
             * SDK Options
             */

            bool debug = false;

            /*
             * Validation
             */

            bool isValid() const;

            void validate() const;
    };
}


#endif