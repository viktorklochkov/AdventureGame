//
// Created by Viktor on 14.07.25.
//
#pragma once

#include "Inventory.hpp"
#include "Room.hpp"

#include <string>
#include <vector>

namespace adv_sk
{

class Player
{
public:
    [[nodiscard]] std::vector<InventoryItem>& get_inventory()
    {
        return _inventory;
    }

    void add_to_inventory(const InventoryItem& item)
    {
        _inventory.push_back(item);
    }

    [[nodiscard]] RoomName get_current_room() const
    {
        return _current_room;
    }

    void change_room(const RoomName& room)
    {
        _current_room = room;
    }

private:
    RoomName _current_room{};
    std::vector<InventoryItem> _inventory{};
};
} // namespace adv_sk
