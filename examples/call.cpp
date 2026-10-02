#include <iostream>

#include "hayavo/meet/config.h"
#include "hayavo/meet/http/http_client.h"
#include "hayavo/meet/services/call_service.h"
#include "config.h"

using namespace hayavo::meet;


class Call{

    public:
        Call():
            call(client.http, client.config)
            {

            }

    private:
        ClientConfig client;
        CallService call;
        

    public:
        void start()
        {

            try
            {

                json payload;

                payload["caller_id"] = "mycaller";
                payload["callee_id"] = "yourcallee";

                auto response = call.start(payload);

                std::cout << "HTTP Status :"
                            << response.statusCode
                            << std::endl;

                if (response.success())
                {
                    std::cout << response.data.dump(4)
                                << std::endl;

                    hostToken(response.data["call_id"].get<std::string>());
                }
                else
                {
                    std::cout << response.body
                                << std::endl;
                }
            }
            catch (const std::exception &e)
            {
                std::cerr
                    << "Error: "
                    << e.what()
                    << std::endl;
            }

        
        };

        void hostToken(const std::string& callID)
        {
            
            try{
                
                auto response = call.hostToken(callID);

                std::cout << "HTTP Status :"
                        << response.statusCode
                        << std::endl;

                if(response.success()){
                    std::cout << response.data.dump(4)
                            << std::endl;
                }else{
                    std::cout << response.body
                            << std::endl;
                }
            }
            catch (const std::exception &e)
            {
                std::cerr
                    << "Error: "
                    << e.what()
                    << std::endl;
            }
           
    }
};



int main(){

    Call service;

    service.start();

    return 0;
}