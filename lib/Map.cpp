//
// Created by Viktor on 14.07.25.
//

#include "Map.hpp"
#include "Direction.hpp"
#include "Inventory.hpp"// for InventoryItem
#include "Room.hpp"

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>// for pair

namespace adv_sk {

Map::Map(const std::vector<Room>& rooms,
         const std::unordered_map<RoomName, RoomConnections>& connections) {
  for (const auto& room : rooms) {
    _rooms.emplace(room.get_name(), room);
  }
  for (const auto& [room_name, connection] : connections) {
    for (const auto& [direction, room_name_to] : connection.connections) {
      _rooms.find(room_name)->second.add_connection(direction,
                                                    room_name_to);
      _rooms.find(room_name_to)
          ->second.add_connection(opposite_direction(direction), room_name);
    }
  }
}

std::optional<RoomName> Map::next_room(const RoomName& current_room,
                                       Direction direction) {
  auto room = _rooms.find(current_room)->second;

  if (!room.connections().connections.contains(direction)) {
    return std::nullopt;
  }
  return room.connections().connections.find(direction)->second;
}

std::string Map::get_welcome_message(const RoomName& room) const {
  return _rooms.find(room)->second.get_message();
}

std::unique_ptr<adv_sk::Map> create_map() {
  InventoryItem const sword("rusty sword");
  InventoryItem const chalice("golden chalice",
                              "You hold the golden chalice aloft. It glints in the "
                              "light and feels cool to the touch.\n");

  Room const grand_hall("GrandHall",
                        "You are in the Grand Hall. It is a vast, echoing chamber.",
                        {chalice});
  Room const armory("Armoury",
                    "You are in the Armoury. Racks of dusty weapons line the walls.",
                    {sword});
  RoomConnections grand_hall_connection;
  grand_hall_connection.add(Direction::North, "Armoury");

  auto map =
      std::make_unique<Map>(std::vector<Room>{grand_hall, armory},
                            std::unordered_map<RoomName, RoomConnections>{
                                {"GrandHall", grand_hall_connection}});

  return map;
}

}// namespace adv_sk
