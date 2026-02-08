//
// Created by Viktor on 14.07.25.
//
#pragma once

#include "IPlayer.hpp"    // for IPlayer
#include "Inventory.hpp"  // for InventoryItem
#include "Types.hpp"      // for RoomName

#include <string>  // for string
#include <vector>  // for vector

namespace adv_sk {

  class Player : public IPlayer {
   public:
    [[nodiscard]] const std::vector<InventoryItem>& get_inventory()
        const override {
      return _inventory;
    }

    [[nodiscard]] std::vector<InventoryItem>& get_mutable_inventory() override {
      return _inventory;
    }

    void add_to_inventory(const InventoryItem& item) override {
      _inventory.push_back(item);
    }

    [[nodiscard]] RoomName get_current_room() const override {
      return _current_room;
    }

    void change_room(const RoomName& room) override {
      _current_room = room;
    }

   private:
    RoomName _current_room{};
    std::vector<InventoryItem> _inventory{};
  };

}  // namespace adv_sk
