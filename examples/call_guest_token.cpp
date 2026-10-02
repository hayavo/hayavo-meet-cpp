#include <iostream>

#include "hayavo/meet/services/call_service.h"
#include "config.h"

using namespace hayavo::meet;


class CallGuestToken{

    public:
        CallGuestToken() :
            call(client.http, client.config)
        {

        }
            
    
    private:
        ClientConfig client;
        CallService call;


    public:
        void token(const std::string &callID){

            try{

                auto response = call.guestToken(callID, client.config.appId);

                std::cout << "HTTP Status :"
                          << response.statusCode
                          << std::endl;

                if (response.success())
                {
                    std::cout << response.data.dump(4)
                              << std::endl;
                }
                else
                {
                    std::cout << response.body
                              << std::endl;
                }
            }catch(const std::exception &e){
                std::cerr << "Error: "
                          << e.what()
                          << std::endl;
            }
            

        }

};


int main(){

    CallGuestToken guest;
   
    std::string call_id;
    std::cout << "Enter callID\n";

    std::getline(std::cin, call_id);

    if(call_id.empty()){
        std::cerr << "Call ID cannot be empty." << std::endl;
        return 1;
    }
    
    guest.token(call_id);

    return 0;
}

