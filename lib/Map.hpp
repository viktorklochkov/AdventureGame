//
// Created by Viktor on 14.07.25.
//

#pragma once

#include "IMap.hpp"   // for IMap
#include "Room.hpp"   // for Room, RoomConnections
#include "Types.hpp"  // for RoomName

#include <memory>         // for unique_ptr
#include <string>         // for string
#include <unordered_map>  // for unordered_map
#include <vector>         // for vector

namespace adv_sk {

  class Map : public IMap {
   public:
    Map(const std::vector<Room>& rooms,
        const std::unordered_map<RoomName, RoomConnections>& connections);

    std::optional<RoomName> next_room(const RoomName& current_room,
                                      Direction direction) override;

    [[nodiscard]] std::string get_welcome_message(
        const RoomName& room) const override;

    [[nodiscard]] Room& get_room(const RoomName& room) override {
      return _rooms.at(room);
    }

   private:
    std::unordered_map<RoomName, Room> _rooms{};
  };

  std::unique_ptr<Map> create_map();

}  // namespace adv_sk
