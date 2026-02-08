#pragma once

#include "IPlayer.hpp"    // for IPlayer
#include "Inventory.hpp"  // for InventoryItem
#include "Types.hpp"      // for RoomName
#include "gmock/gmock.h"  // for MOCK_METHOD

#include <vector>  // for vector

namespace adv_sk::test {

  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  class MockPlayer : public IPlayer {
   public:
    MOCK_METHOD(const std::vector<InventoryItem>&, get_inventory, (),
                (const, override));
    MOCK_METHOD(std::vector<InventoryItem>&, get_mutable_inventory, (),
                (override));
    MOCK_METHOD(void, add_to_inventory, (const InventoryItem& item),
                (override));
    MOCK_METHOD(RoomName, get_current_room, (), (const, override));
    MOCK_METHOD(void, change_room, (const RoomName& room), (override));
  };
  // NOLINTEND(misc-non-private-member-variables-in-classes)

}  // namespace adv_sk::test
