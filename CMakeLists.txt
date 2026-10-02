cmake_minimum_required(VERSION 3.14)

project(
    hayavo-meet-cpp-sdk
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# ---------------------------------------------------------
# nlohmann/json
# ---------------------------------------------------------

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(nlohmann_json)

# ---------------------------------------------------------
# CURL
# ---------------------------------------------------------

find_package(CURL REQUIRED)

# ---------------------------------------------------------
# Hayavo Meet SDK - HTTP
# ---------------------------------------------------------

add_library(hayavo_meet STATIC

    
    src/config.cpp

    src/http/http_client.cpp

    src/services/auth_service.cpp
    src/services/room_service.cpp
    src/services/call_service.cpp
    src/services/admin_service.cpp
    src/services/attachment_service.cpp
    src/utils/validator.cpp
)

target_include_directories(
    hayavo_meet
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
    hayavo_meet
    PUBLIC
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

# ---------------------------------------------------------
# Examples
# ---------------------------------------------------------

add_executable(
    room_example
    examples/room.cpp
)

target_include_directories(
    room_example
    PRIVATE
        ${PROJECT_SOURCE_DIR}/examples
)

target_link_libraries(
    room_example
    PRIVATE
        hayavo_meet
)

# ---------------------------------------------------------

add_executable(
    guest_token_example
    examples/guest_token.cpp
)

target_include_directories(
    guest_token_example
    PRIVATE
        ${PROJECT_SOURCE_DIR}/examples
)

target_link_libraries(
    guest_token_example
    PRIVATE
        hayavo_meet
)

# ---------------------------------------------------------

add_executable(
    call_example
    examples/call.cpp
)

target_include_directories(
    call_example
    PRIVATE
        ${PROJECT_SOURCE_DIR}/examples
)

target_link_libraries(
    call_example
    PRIVATE
        hayavo_meet
)

# ---------------------------------------------------------

add_executable(
    call_guest_token_example
    examples/call_guest_token.cpp
)

target_include_directories(
    call_guest_token_example
    PRIVATE
        ${PROJECT_SOURCE_DIR}/examples
)

target_link_libraries(
    call_guest_token_example
    PRIVATE
        hayavo_meet
)