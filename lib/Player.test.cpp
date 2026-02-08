// Player unit tests

#include "Player.hpp"

#include "Inventory.hpp"  // for InventoryItem
#include "gtest/gtest.h"  // for TEST, EXPECT_EQ

#include <string>  // for string
#include <vector>  // for vector

namespace adv_sk::test {

  TEST(Player, initiallyEmptyInventory) {
    const Player player;
    EXPECT_TRUE(player.get_inventory().empty());
  }

  TEST(Player, initialCurrentRoomIsEmpty) {
    const Player player;
    EXPECT_EQ(player.get_current_room(), "");
  }

  TEST(Player, changeRoomUpdatesCurrentRoom) {
    Player player;
    player.change_room("GrandHall");
    EXPECT_EQ(player.get_current_room(), "GrandHall");
  }

  TEST(Player, addToInventory) {
    Player player;
    player.add_to_inventory(InventoryItem{.name = "sword"});
    EXPECT_EQ(player.get_inventory().size(), 1);
    EXPECT_EQ(player.get_inventory()[0].name, "sword");
  }

  TEST(Player, getMutableInventoryAllowsModification) {
    Player player;
    player.add_to_inventory(InventoryItem{.name = "sword"});
    auto& inventory = player.get_mutable_inventory();
    inventory.clear();
    EXPECT_TRUE(player.get_inventory().empty());
  }

  TEST(Player, addMultipleItems) {
    Player player;
    player.add_to_inventory(InventoryItem{.name = "sword"});
    player.add_to_inventory(InventoryItem{.name = "shield"});
    EXPECT_EQ(player.get_inventory().size(), 2);
  }

  TEST(Player, changeRoomMultipleTimes) {
    Player player;
    player.change_room("GrandHall");
    player.change_room("Armoury");
    EXPECT_EQ(player.get_current_room(), "Armoury");
  }

}  // namespace adv_sk::test
