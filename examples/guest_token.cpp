#include <iostream>

#include "hayavo/meet/config.h"
#include "hayavo/meet/http/http_client.h"
#include "hayavo/meet/services/room_service.h"
#include "config.h"

using namespace hayavo::meet;


class GuestToken{

    public:
        GuestToken():
            room(client.http)
            {

            }

    private:
        ClientConfig client;
        RoomService room;

    public:
        void token(){

            try{

                auto response = room.guestToken("hayavo_meet_default_room", client.config.appId);

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

    GuestToken guest;

    guest.token();

    return 0;
    
}