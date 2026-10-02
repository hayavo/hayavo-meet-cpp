#include "hayavo/meet/services/attachment_service.h"

#include <filesystem>
#include <map>
#include <stdexcept>

namespace fs = std::filesystem;

namespace hayavo::meet
{

    /*
    |------------------------------------  
    | Constructor
    |------------------------------------
    */

    AttachmentService:: AttachmentService(HTTPClient &http)
    : http_(http)
    {

    }

    /*
    |--------------------------------------------------------------------------
    | Check File Exists
    |--------------------------------------------------------------------------
    */

    bool AttachmentService::fileExists(
        const std::string &filePath) const
    {
        return fs::exists(filePath) &&
               fs::is_regular_file(filePath);
    }

    /*
    |--------------------------------------------------------------------------
    | Get File Size
    |--------------------------------------------------------------------------
    */

    std::uintmax_t AttachmentService::fileSize(
        const std::string &filePath) const
    {
        return fs::file_size(filePath);
    }

    /*
    |------------------------------------
    | Upload Attachment
    | 
    |------------------------------------
    */

    HttpResponse AttachmentService::upload(
        const std::string &token,
        const std::string &roomId,
        const std::string &filePath,
        const std::string &mimeType)
    {

        if (token.empty())
        {
            throw std::invalid_argument(
                "Token is required.");
        }

        if (roomId.empty())
        {
            throw std::invalid_argument(
                "Room ID is required.");
        }

        if (filePath.empty())
        {
            throw std::invalid_argument(
                "File path is required.");
        }

        if (!fileExists(filePath))
        {
            throw std::runtime_error(
                "File does not exist.");
        }

        if (fileSize(filePath) > MAX_FILE_SIZE)
        {
            throw std::runtime_error(
                "Maximum file size is 1024 MB.");
        }

        std::map<std::string, std::string> fields;

        fields["room_id"] = roomId;

        std::map<std::string, std::string> headers;

        headers["Authorization"] = "Bearer " + token;
        

        return http_.upload(
            "/attachments/upload",
            filePath,
            fields,
            headers,
            false);
    }

    /*
    |--------------------------------------------------------------------------
    | Download Attachment
    |--------------------------------------------------------------------------
    |
    | 
    |
    */

    HttpResponse AttachmentService::download(
        const std::string &attachmentId,
        const std::string &savePath,
        const std::string &token)
    {
        if (attachmentId.empty())
        {
            throw std::invalid_argument(
                "Attachment ID is required.");
        }

        if (savePath.empty())
        {
            throw std::invalid_argument(
                "Save path is required.");
        }

        throw std::runtime_error(
            "Attachment download is not implemented yet.");
    }
}