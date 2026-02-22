//
// Created by Viktor on 14.07.25.
//

#include "Map.hpp"

#include "Direction.hpp"
#include "Inventory.hpp"  // for InventoryItem
#include "Room.hpp"

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>  // for pair

namespace adv_sk {

  Map::Map(const std::vector<Room>& rooms,
           const std::unordered_map<RoomName, RoomConnections>& connections) {
    for (const auto& room : rooms) {
      _rooms.emplace(room.get_name(), room);
    }
    for (const auto& [room_name, connection] : connections) {
      for (const auto& [direction, room_name_to] : connection.connections) {
        _rooms.find(room_name)->second.add_connection(direction, room_name_to);
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
    const InventoryItem sword{
        .name = "rusty sword",
        .use_message =
            "You swing the rusty sword. The blade holds, but barely.\n"};
    const InventoryItem chalice{
        .name = "golden chalice",
        .use_message = "The chalice gleams. It belongs on the throne.\n"};
    const InventoryItem tome{
        .name = "old tome",
        .use_message =
            "The tome describes a hidden cellar beneath the kitchen.\n"};
    const InventoryItem key{
        .name = "iron key",
        .use_message = "The key is cold and heavy in your hand.\n"};
    const InventoryItem pendant{
        .name = "silver pendant",
        .use_message = "The pendant pulses with faint warmth.\n"};

    const Room grand_hall(
        "GrandHall",
        "You are in the Grand Hall. It is a vast, echoing chamber.", {chalice});
    const Room armoury(
        "Armoury",
        "You are in the Armoury. Racks of dusty weapons line the walls.",
        {sword});
    const Room library(
        "Library",
        "You are in the Library. Shelves of dusty books surround you.", {tome});
    const Room throne_room(
        "ThroneRoom",
        "You are in the Throne Room. An ornate throne sits at the far end.");
    const Room kitchen(
        "Kitchen",
        "You are in the Kitchen. The smell of old ash lingers in the air.",
        {key});
    const Room cellar("Cellar", "You are in the Cellar. It is dark and damp.",
                      {pendant});
    const Room barracks(
        "Barracks",
        "You are in the Barracks. Rows of empty bunks line the walls.");

    RoomConnections grand_hall_connections;
    grand_hall_connections.add(Direction::North, "Armoury");
    grand_hall_connections.add(Direction::West, "Library");
    grand_hall_connections.add(Direction::East, "ThroneRoom");
    grand_hall_connections.add(Direction::South, "Kitchen");

    RoomConnections armoury_connections;
    armoury_connections.add(Direction::East, "Barracks");

    RoomConnections kitchen_connections;
    kitchen_connections.add(Direction::South, "Cellar");

    return std::make_unique<Map>(
        std::vector<Room>{grand_hall, armoury, library, throne_room, kitchen,
                          cellar, barracks},
        std::unordered_map<RoomName, RoomConnections>{
            {"GrandHall", grand_hall_connections},
            {"Armoury", armoury_connections},
            {"Kitchen", kitchen_connections}});
  }

}  // namespace adv_sk
