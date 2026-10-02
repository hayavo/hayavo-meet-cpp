# Hayavo Meet C++ SDK

C++ SDK for interacting with the Hayavo Meet API.

This SDK currently provides examples for:

* Create a room
* Generate a host token
* Generate a guest token

## Documentation

For complete Hayavo Meet API and SDK documentation:

**https://docs.meet.hayavo.com**

## Requirements

* C++17 or later
* CMake 3.14 or later
* libcurl
* nlohmann/json

## Build

Clone the repository and build with CMake:

```bash
git clone https://github.com/hayavo/hayavo-meet-cpp.git
cd hayavo-meet-cpp-sdk

cmake -S . -B build
cmake --build build -j$(nproc)
```

## Configuration

The examples use the Hayavo Meet application configuration.

Configure your credentials in the example configuration:

```cpp
config.apiKey = "YOUR_API_KEY";
config.appId = "YOUR_APP_ID";
config.appSecret = "YOUR_APP_SECRET";
config.appPlatform = "YOUR_PLATFORM";
config.appIdentifier = "YOUR_IDENTIFIER";
config.appName = "Your Application";
```

Do not commit real API keys or API secrets to GitHub.

---

# Room Create

The room service can be used to create a new meeting room.

Example:

```cpp
#include "hayavo/meet/services/room_service.h"
#include "config.h"

using namespace hayavo::meet;

ClientConfig client;
RoomService room(client.http);

auto response = room.create();

std::cout << "HTTP Status: "
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
```

Build and run the room example:

```bash
cmake -S . -B build
cmake --build build -j$(nproc)

./build/room_example
```

A successful request returns the room information from the Hayavo Meet API.

---

# Host Token

After creating a room, a host token can be generated for the room.

The host token is used by the host to join the meeting room.

Example:

```cpp
auto response = room.hostToken(roomId);
```

Example handling:

```cpp
auto response = room.hostToken(roomId);

std::cout << "HTTP Status: "
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
```

The `roomId` should be the ID returned when the room was created.

---

# Guest Token

A guest token can be generated for a guest joining a room.

Example:

```cpp
auto response = room.guestToken(
    roomId,
    client.config.appId
);
```

Example handling:

```cpp
auto response = room.guestToken(
    roomId,
    client.config.appId
);

std::cout << "HTTP Status: "
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
```

The `roomId` should be the ID of the room the guest wants to join.

---

# Room Flow

The basic Hayavo Meet room flow is:

```text
Create Room
     │
     ▼
Room ID
     │
     ├──────────────► Host Token
     │                    │
     │                    ▼
     │               Host joins room
     │
     └──────────────► Guest Token
                          │
                          ▼
                     Guest joins room
```

## License

See the `LICENSE` file for license information.

## Changelog

See `CHANGELOG.md` for version history.
