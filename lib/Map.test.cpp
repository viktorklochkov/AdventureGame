// Map unit tests

#include "Map.hpp"

#include "Direction.hpp"  // for Direction, opposite_direction
#include "Inventory.hpp"  // for InventoryItem
#include "Room.hpp"       // for Room, RoomConnections
#include "Types.hpp"      // for RoomName
#include "gtest/gtest.h"  // for TEST, EXPECT_EQ

#include <memory>         // for unique_ptr, make_unique
#include <optional>       // for optional
#include <string>         // for string
#include <unordered_map>  // for unordered_map
#include <utility>        // for pair
#include <vector>         // for vector

namespace adv_sk::test {

  namespace {
    std::unique_ptr<Map> make_test_map() {
      const Room grand_hall("GrandHall", "Welcome to the Grand Hall.",
                            {InventoryItem{.name = "chalice"}});
      const Room armory("Armoury", "Welcome to the Armoury.",
                        {InventoryItem{.name = "sword"}});

      RoomConnections grand_hall_connections;
      grand_hall_connections.add(Direction::North, "Armoury");

      return std::make_unique<Map>(
          std::vector<Room>{grand_hall, armory},
          std::unordered_map<RoomName, RoomConnections>{
              {"GrandHall", grand_hall_connections}});
    }
  }  // namespace

  TEST(Map, nextRoomReturnsConnectedRoom) {
    auto map = make_test_map();
    auto result = map->next_room("GrandHall", Direction::North);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "Armoury");
  }

  TEST(Map, nextRoomReturnsNulloptForNoConnection) {
    auto map = make_test_map();
    auto result = map->next_room("GrandHall", Direction::East);
    EXPECT_FALSE(result.has_value());
  }

  TEST(Map, bidirectionalConnectionsCreated) {
    auto map = make_test_map();
    auto result = map->next_room("Armoury", Direction::South);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "GrandHall");
  }

  TEST(Map, getWelcomeMessage) {
    auto map = make_test_map();
    EXPECT_EQ(map->get_welcome_message("GrandHall"),
              "Welcome to the Grand Hall.");
    EXPECT_EQ(map->get_welcome_message("Armoury"), "Welcome to the Armoury.");
  }

  TEST(Map, getRoomReturnsMutableReference) {
    auto map = make_test_map();
    auto& room = map->get_room("GrandHall");
    EXPECT_EQ(room.get_name(), "GrandHall");
    room.add_to_inventory(InventoryItem{.name = "potion"});
    EXPECT_EQ(map->get_room("GrandHall").inventory().size(), 2);
  }

  TEST(Map, createMapFactoryReturnsValidMap) {
    auto map = create_map();
    EXPECT_EQ(map->get_welcome_message("GrandHall"),
              "You are in the Grand Hall. It is a vast, echoing chamber.");
    EXPECT_EQ(map->get_welcome_message("Armoury"),
              "You are in the Armoury. Racks of dusty weapons line the "
              "walls.");
    auto result = map->next_room("GrandHall", Direction::North);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "Armoury");
  }

  TEST(Map, createMapFactoryHasBidirectionalConnections) {
    auto map = create_map();
    auto result = map->next_room("Armoury", Direction::South);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "GrandHall");
  }

}  // namespace adv_sk::test
