#pragma once

#include "Inventory.hpp"  // for InventoryItem
#include "Types.hpp"      // for RoomName

#include <vector>  // for vector

namespace adv_sk {

  class IPlayer {
   public:
    virtual ~IPlayer() = default;

    [[nodiscard]] virtual const std::vector<InventoryItem>& get_inventory()
        const = 0;

    [[nodiscard]] virtual std::vector<InventoryItem>&
    get_mutable_inventory() = 0;

    virtual void add_to_inventory(const InventoryItem& item) = 0;

    [[nodiscard]] virtual RoomName get_current_room() const = 0;

    virtual void change_room(const RoomName& room) = 0;
  };

}  // namespace adv_sk
