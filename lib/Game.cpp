//
// Created by Viktor on 14.07.25.
//

#include "Game.hpp"

#include <format>
#include <iostream>

namespace adv_sk
{

namespace detail
{
std::string read_from_cin()
{
    std::string result;
    std::cin >> result;
    return result;
}
} // namespace detail

void Game::handle_user_action() // NOLINT(misc-no-recursion)
{
    auto action = detail::read_from_cin();

    if (action == "quit")
    {
        exit(EXIT_SUCCESS);
    }
    else if (action == "move")
    {
        std::cout << "Where do you want to go?" << std::endl;
        auto directions = get_available_directions();
        std::cout << "Available directions: \n";
        for (const auto& direction : directions)
        {
            std::cout << direction_to_string(direction) << "\n";
        }
        std::string direction_string;
        std::cin >> direction_string;
        try
        {
            move(string_to_direction(direction_string));
        }
        catch (std::runtime_error& e)
        {
            std::cout << "Unknown direction! Available directions: \n";
            for (const auto& direction : directions)
            {
                std::cout << direction_to_string(direction) << "\n";
            }
        }
    }
    else
    {
        update_message("Unknown command\n");
    }
    handle_user_action();
}

void Game::start()
{
    init();
    update_message("");

    if (!_test_mode)
    {
        handle_user_action();
    }
}

void Game::move(Direction direction)
{
    const auto next_room = _map->nextRoom(_player.get_current_room(), direction);
    if (next_room.has_value())
    {
        _player.change_room(next_room.value());
        update_message("");
    }
    else
    {
        update_message("Wrong direction!\n");
    }
}

void Game::investigate()
{
    auto room = _player.get_current_room();
    auto& inventory = _map->get_room(room).inventory();
    if (!inventory.empty())
    {
        std::string message = "You search the room. You found";
        for (auto& item : inventory)
        {
            item.is_visible = true;
            message.append(" a " + item.name);
        }
        message.append("!\n");
        update_message(message);
    }
    else
    {
        update_message("You search the room. Nothing found!\n");
    }
}

void Game::take_item(const std::string& item_name)
{
    const auto room = _player.get_current_room();
    const auto& inventory = _map->get_room(room).inventory();
    for (auto& item : inventory)
    {
        if (item.name == item_name && item.is_visible)
        {
            std::cout << "You take the " << item.name << "!\n";
            _player.add_to_inventory(item);
            _map->get_room(room).remove_from_inventory(item);
            break;
        }
        else
        {
            std::cout << "You can't take the " << item.name << "!\n";
        }
    }
}

void Game::display_player_inventory()
{
    std::string message("Your inventory contains:");
    for (const auto& item : _player.get_inventory())
    {
        message.append(" " + item.name);
    }
    message.append(".\n");
    update_message(message);
}

void Game::use_item(const std::string& item_name)
{
    auto& inventory = _player.get_inventory();
    auto it =
        std::ranges::find_if(inventory, [&item_name](const InventoryItem& item)
                             { return item.name == item_name; });
    if (it != inventory.end())
    {
        update_message(it->use_message);
        inventory.erase(it);
    }
    else
    {
        update_message("You can't use the " + item_name + "!\n");
    }
}

void Game::drop_item(const std::string& item_name)
{
    auto& inventory = _player.get_inventory();
    auto it =
        std::ranges::find_if(inventory, [&item_name](const InventoryItem& item)
                             { return item.name == item_name; });
    if (it != inventory.end())
    {
        update_message(std::format(
            "You drop the {}. It fades away in the darkness.\n", item_name));
        _map->get_room(_player.get_current_room()).add_to_inventory(*it);
        inventory.erase(it);
    }
    else
    {
        update_message("You can't drop the " + item_name + "!\n");
    }
}

std::vector<Direction> Game::get_available_directions() const
{
    std::vector<Direction> result;
    for (auto direction :
         {Direction::North, Direction::South, Direction::East, Direction::West})
    {
        auto room = _map->nextRoom(_player.get_current_room(), direction);
        if (room.has_value())
        {
            result.push_back(direction);
        }
    }
    return result;
}

void Game::print_message() const
{
    std::cout << _current_message << std::endl;
}

void Game::init()
{
    // default map
    if (_map == nullptr)
    {
        _map = create_map();
    }

    _player.change_room("GrandHall");
}

void Game::update_message(const std::string& message)
{
    if (message.empty())
    {
        _current_message = _map->get_welcome_message(_player.get_current_room());
    }
    else
    {
        _current_message = message;
    }
    print_message();
}
} // namespace adv_sk
