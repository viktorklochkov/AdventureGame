//
// Created by Viktor on 14.07.25.
//

#pragma once

#include "Room.hpp"

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace adv_sk
{


class Map
{
public:
    Map(const std::vector<Room>& rooms,
        const std::unordered_map<RoomName, RoomConnections>& map);

    std::optional<RoomName> nextRoom(const RoomName& current_room,
                                     Direction direction);

    [[nodiscard]] std::string get_welcome_message(const RoomName& room) const;

    [[nodiscard]] Room& get_room(const RoomName& room)
    {
        return _rooms.at(room);
    }

private:
    std::unordered_map<RoomName, Room> _rooms{};
};

std::unique_ptr<Map> create_map();

} // namespace adv_sk
