#include <iostream>
#include <string>

#include "hayavo/meet/services/attachment_service.h"
#include "config.h"

using namespace hayavo::meet;

class UploadAttachment
{
    public:
        UploadAttachment()
            : attachment(client.http)
        {
        }

    private:
        ClientConfig client;
        AttachmentService attachment;
    

    
    public:
        void upload(
            const std::string &token,
            const std::string &roomId,
            const std::string &filePath)
        {
            try
            {
                auto response = attachment.upload(token, roomId, filePath);

                std::cout
                    << "HTTP Status : "
                    << response.statusCode
                    << std::endl;

                if (response.success())
                {
                    std::cout
                        << response.data.dump(4)
                        << std::endl;
                }
                else
                {
                    std::cout
                        << response.body
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

int main()
{
    UploadAttachment uploader;

    std::string token;
    std::string roomId;
    std::string filePath;

    std::cout << "Attachment Token : ";
    std::getline(std::cin, token);

    std::cout << "Room ID : ";
    std::getline(std::cin, roomId);

    std::cout << "File Path : ";
    std::getline(std::cin, filePath);

    uploader.upload(
        token,
        roomId,
        filePath);

    return 0;
}