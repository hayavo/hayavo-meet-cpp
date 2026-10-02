#ifndef HAYAVO_MEET_SERVICES_ATTACHMENT_SERVICE_H
#define HAYAVO_MEET_SERVICES_ATTACHMENT_SERVICE_H

#include <string>

#include "../http/http_client.h"
#include "../json/single_include/nlohmann/json.hpp"

namespace hayavo::meet
{

    /*
    |--------------------------------------------------------------------------
    | Attachment Service
    |--------------------------------------------------------------------------
    |
    | Upload and download attachments.
    |
    */

    class AttachmentService
    {
    public:
        /*
        |--------------------------------------------------------------------------
        | Constants
        |--------------------------------------------------------------------------
        */

        static constexpr std::size_t MAX_FILE_SIZE =
            1024ULL * 1024ULL * 1024ULL; 

        /*
        |--------------------------------------------------------------------------
        | Constructor
        |--------------------------------------------------------------------------
        */

        explicit AttachmentService(HTTPClient &http);

        ~AttachmentService() = default;

        AttachmentService(const AttachmentService &) = delete;
        AttachmentService &operator=(const AttachmentService &) = delete;

        /*
        |--------------------------------------------------------------------------
        | Upload Attachment
        |--------------------------------------------------------------------------
        |
        | 
        |
        */

        HttpResponse upload(
            const std::string &token,
            const std::string &roomId,
            const std::string &filePath,
            const std::string &mimeType =
                "application/octet-stream");

        /*
        |--------------------------------------------------------------------------
        | Download Attachment
        |--------------------------------------------------------------------------
        |
        | 
        |
        */

        HttpResponse download(
            const std::string &attachmentId,
            const std::string &savePath,
            const std::string &token = "");

    private:
        HTTPClient &http_;

        /*
        |--------------------------------------------------------------------------
        | File Validation
        |--------------------------------------------------------------------------
        */

        bool fileExists(
            const std::string &filePath) const;

        std::uintmax_t fileSize(
            const std::string &filePath) const;
    };

}

#endif