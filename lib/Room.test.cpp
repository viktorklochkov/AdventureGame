// Room and RoomConnections unit tests

#include "Room.hpp"

#include "Direction.hpp"  // for Direction
#include "Inventory.hpp"  // for InventoryItem
#include "gtest/gtest.h"  // for TEST, EXPECT_EQ

#include <optional>       // for optional
#include <string>         // for string
#include <unordered_map>  // for unordered_map
#include <vector>         // for vector

namespace adv_sk::test {

  // --- RoomConnections tests ---

  TEST(RoomConnections, addAndGetConnection) {
    RoomConnections connections;
    connections.add(Direction::North, "Armoury");
    auto result = connections.get_connection(Direction::North);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "Armoury");
  }

  TEST(RoomConnections, getConnectionReturnsNulloptForMissing) {
    const RoomConnections connections;
    auto result = connections.get_connection(Direction::North);
    EXPECT_FALSE(result.has_value());
  }

  TEST(RoomConnections, addMultipleConnections) {
    RoomConnections connections;
    connections.add(Direction::North, "Armoury");
    connections.add(Direction::South, "GrandHall");
    auto north = connections.get_connection(Direction::North);
    ASSERT_TRUE(north.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(north.value(), "Armoury");
    auto south = connections.get_connection(Direction::South);
    ASSERT_TRUE(south.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(south.value(), "GrandHall");
    EXPECT_FALSE(connections.get_connection(Direction::East).has_value());
  }

  // --- Room tests ---

  TEST(Room, constructorSetsNameAndMessage) {
    const Room room("GrandHall", "A vast chamber.");
    EXPECT_EQ(room.get_name(), "GrandHall");
    EXPECT_EQ(room.get_message(), "A vast chamber.");
  }

  TEST(Room, defaultMessageIsEmpty) {
    const Room room("TestRoom");
    EXPECT_EQ(room.get_message(), "");
  }

  TEST(Room, inventoryAccessIsMutable) {
    Room room("R", "", {InventoryItem{.name = "sword"}});
    EXPECT_EQ(room.inventory().size(), 1);
    room.inventory()[0].is_visible = true;
    EXPECT_TRUE(room.inventory()[0].is_visible);
  }

  TEST(Room, addToInventory) {
    Room room("R");
    EXPECT_TRUE(room.inventory().empty());
    room.add_to_inventory(InventoryItem{.name = "shield"});
    EXPECT_EQ(room.inventory().size(), 1);
    EXPECT_EQ(room.inventory()[0].name, "shield");
  }

  TEST(Room, removeFromInventory) {
    const InventoryItem sword{.name = "sword"};
    Room room("R", "", {sword});
    EXPECT_EQ(room.inventory().size(), 1);
    room.remove_from_inventory(sword);
    EXPECT_TRUE(room.inventory().empty());
  }

  TEST(Room, addConnectionAndRetrieve) {
    Room room("R");
    room.add_connection(Direction::North, "Armoury");
    auto conns = room.connections();
    auto result = conns.get_connection(Direction::North);
    ASSERT_TRUE(result.has_value());
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    EXPECT_EQ(result.value(), "Armoury");
  }

  TEST(Room, connectionsReturnsCopy) {
    Room room("R");
    room.add_connection(Direction::North, "Armoury");
    auto conns = room.connections();
    EXPECT_EQ(conns.connections.size(), 1);
  }

  TEST(Room, constructorWithInventory) {
    const InventoryItem sword{
        .name = "sword", .use_message = "A rusty sword.", .is_visible = false};
    const InventoryItem shield{.name = "shield",
                               .use_message = "A wooden shield.",
                               .is_visible = false};
    Room room("R", "msg", {sword, shield});
    EXPECT_EQ(room.inventory().size(), 2);
    EXPECT_EQ(room.inventory()[0].name, "sword");
    EXPECT_EQ(room.inventory()[1].name, "shield");
  }

}  // namespace adv_sk::test
