#pragma once

#include "Direction.hpp"
#include "Inventory.hpp"

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace adv_sk
{

using RoomName = std::string;

struct RoomConnections
{
    void add(Direction direction, const RoomName& room)
    {
        connections.emplace(direction, room);
    }

    [[nodiscard]] std::optional<RoomName> get_connection(
        Direction direction) const;

    std::unordered_map<Direction, RoomName> connections{};
};

class Room
{
public:
    explicit Room(RoomName _name, std::string _message = {},
                  std::vector<InventoryItem> _inventory = {},
                  RoomConnections _connections = {}) :
        _name(std::move(_name)), _message(std::move(_message)),
        _inventory(std::move(_inventory)), _connections(std::move(_connections))
    {
    }

    [[nodiscard]] std::string get_message() const
    {
        return _message;
    }

    [[nodiscard]] std::string get_name() const
    {
        return _name;
    }

    void add_connection(Direction direction, const RoomName& room)
    {
        _connections.add(direction, room);
    }

    [[nodiscard]] RoomConnections connections() const
    {
        return _connections;
    }

    [[nodiscard]] std::vector<InventoryItem>& inventory()
    {
        return _inventory;
    }

    void add_to_inventory(const InventoryItem& item)
    {
        _inventory.push_back(item);
    }

    void remove_from_inventory(const InventoryItem& item)
    {
        std::erase(_inventory, item);
    }

private:
    RoomName _name;
    std::string _message{};
    std::vector<InventoryItem> _inventory{};

    RoomConnections _connections{};
};

} // namespace adv_sk
