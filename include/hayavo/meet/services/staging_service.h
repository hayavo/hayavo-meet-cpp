#ifndef HAYAVO_MEET_STAGING_CONFIG_H
#define HAYAVO_MEET_STAGING_CONFIG_H

#include <string>

namespace hayavo::meet
{

    struct StagingConfig
    {
        std::string product = "meet";
        std::string node = "node";
        std::string domain = "hayavo.com";
        std::string edgePrefix = "edge";
        std::string core = "core";
        std::string apiVersion = "v1";
        std::string mq = "83m2q";
        

        std::string staging() const
        {
            return "https://api." +
                   product + "." +
                   domain + "/" +
                   apiVersion;
        }

        std::string rtc() const
        {
            return "wss://" +
                   edgePrefix + "." +
                   product + "." +
                   domain;
        }

        std::string signal() const
        {
            return "wss://signal-" +
                   core + "-" +
                   node + "-" +
                   mq + "-" +
                   product + "." +
                   domain +
                   "/ws";
        }
    };

}

#endif