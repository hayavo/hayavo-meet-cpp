#include <iostream>

#include "hayavo/meet/config.h"
#include "hayavo/meet/http/http_client.h"
#include "hayavo/meet/services/room_service.h"
#include "config.h"


using namespace hayavo::meet;


class Service{
    
    public:
        Service():
            room(client.http)
            {

            }

    private:
        ClientConfig client;
        RoomService room;

    public:
        void create(){

            try{

                auto response = room.create("hayavo_meet_default_room", "hm_video_party", 5);

                std::cout << "HTTP Status :"
                          << response.statusCode
                          << std::endl;

                if (response.success())
                {
                    std::cout << response.data.dump(4)
                              << std::endl;

                    hostToken(response.data["room_name"].get<std::string>());

                }else
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


        void hostToken(const std::string& roomID){
            try{
                auto response = room.hostToken(roomID);

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
            }catch(const std::exception &e){
                std::cerr << "Error: "
                          << e.what()
                          << std::endl;
            }
        }
};


int main(){

    Service service;

    service.create();

    return 0;
};