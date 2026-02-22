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

  // --- Phase 2: new room connections ---

  TEST(Map, createMapLibraryReachableFromGrandHall) {
    auto map = create_map();
    auto result = map->next_room("GrandHall", Direction::West);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "Library");
  }

  TEST(Map, createMapThroneRoomReachableFromGrandHall) {
    auto map = create_map();
    auto result = map->next_room("GrandHall", Direction::East);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "ThroneRoom");
  }

  TEST(Map, createMapKitchenReachableFromGrandHall) {
    auto map = create_map();
    auto result = map->next_room("GrandHall", Direction::South);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "Kitchen");
  }

  TEST(Map, createMapBarracksReachableFromArmoury) {
    auto map = create_map();
    auto result = map->next_room("Armoury", Direction::East);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "Barracks");
  }

  TEST(Map, createMapCellarReachableFromKitchen) {
    auto map = create_map();
    const auto kitchen = map->next_room("GrandHall", Direction::South);
    ASSERT_TRUE(kitchen.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    auto result = map->next_room(*kitchen, Direction::South);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "Cellar");
  }

  // --- Phase 2: bidirectional connections for new rooms ---

  TEST(Map, createMapGrandHallReachableFromLibrary) {
    auto map = create_map();
    const auto library = map->next_room("GrandHall", Direction::West);
    ASSERT_TRUE(library.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    auto result = map->next_room(*library, Direction::East);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "GrandHall");
  }

  TEST(Map, createMapGrandHallReachableFromThroneRoom) {
    auto map = create_map();
    const auto throne = map->next_room("GrandHall", Direction::East);
    ASSERT_TRUE(throne.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    auto result = map->next_room(*throne, Direction::West);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "GrandHall");
  }

  TEST(Map, createMapGrandHallReachableFromKitchen) {
    auto map = create_map();
    const auto kitchen = map->next_room("GrandHall", Direction::South);
    ASSERT_TRUE(kitchen.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    auto result = map->next_room(*kitchen, Direction::North);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "GrandHall");
  }

  TEST(Map, createMapArmouryReachableFromBarracks) {
    auto map = create_map();
    const auto barracks = map->next_room("Armoury", Direction::East);
    ASSERT_TRUE(barracks.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    auto result = map->next_room(*barracks, Direction::West);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "Armoury");
  }

  TEST(Map, createMapKitchenReachableFromCellar) {
    auto map = create_map();
    const auto kitchen = map->next_room("GrandHall", Direction::South);
    ASSERT_TRUE(kitchen.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const auto cellar = map->next_room(*kitchen, Direction::South);
    ASSERT_TRUE(cellar.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    auto result = map->next_room(*cellar, Direction::North);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "Kitchen");
  }

  // --- Phase 2: item placement ---

  TEST(Map, createMapGrandHallChaliceHasHintMessage) {
    auto map = create_map();
    const auto& inventory = map->get_room("GrandHall").inventory();
    ASSERT_EQ(inventory.size(), 1);
    EXPECT_EQ(inventory[0].name, "golden chalice");
    EXPECT_EQ(inventory[0].use_message,
              "The chalice gleams. It belongs on the throne.\n");
  }

  TEST(Map, createMapLibraryHasOldTome) {
    auto map = create_map();
    const auto library = map->next_room("GrandHall", Direction::West);
    ASSERT_TRUE(library.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const auto& inventory = map->get_room(*library).inventory();
    ASSERT_EQ(inventory.size(), 1);
    EXPECT_EQ(inventory[0].name, "old tome");
  }

  TEST(Map, createMapKitchenHasIronKey) {
    auto map = create_map();
    const auto kitchen = map->next_room("GrandHall", Direction::South);
    ASSERT_TRUE(kitchen.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const auto& inventory = map->get_room(*kitchen).inventory();
    ASSERT_EQ(inventory.size(), 1);
    EXPECT_EQ(inventory[0].name, "iron key");
  }

  TEST(Map, createMapCellarHasSilverPendant) {
    auto map = create_map();
    const auto kitchen = map->next_room("GrandHall", Direction::South);
    ASSERT_TRUE(kitchen.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const auto cellar = map->next_room(*kitchen, Direction::South);
    ASSERT_TRUE(cellar.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const auto& inventory = map->get_room(*cellar).inventory();
    ASSERT_EQ(inventory.size(), 1);
    EXPECT_EQ(inventory[0].name, "silver pendant");
  }

  TEST(Map, createMapThroneRoomHasNoItems) {
    auto map = create_map();
    const auto throne = map->next_room("GrandHall", Direction::East);
    ASSERT_TRUE(throne.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_TRUE(map->get_room(*throne).inventory().empty());
  }

  TEST(Map, createMapBarracksHasNoItems) {
    auto map = create_map();
    const auto barracks = map->next_room("Armoury", Direction::East);
    ASSERT_TRUE(barracks.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_TRUE(map->get_room(*barracks).inventory().empty());
  }

}  // namespace adv_sk::test
