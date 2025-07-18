//
// Created by Viktor on 14.07.25.
//

#pragma once

#include "Direction.hpp"
#include "Map.hpp"
#include "Player.hpp"

#include <memory>
#include <string>
#include <vector>

namespace adv_sk
{

class IInputHandler
{
};

class Game
{
public:
    Game() = default;
    explicit Game(std::unique_ptr<Map> map) : _map(std::move(map))
    {
    }

    void handle_user_action();

    void start();

    void move(Direction direction);

    void investigate();

    void take_item(const std::string& item_name);

    void display_player_inventory();

    void use_item(const std::string& item_name);

    void drop_item(const std::string& item_name);

    [[nodiscard]] std::vector<Direction> get_available_directions() const;

    [[nodiscard]] std::string get_current_message() const
    {
        return _current_message;
    }

    [[nodiscard]] RoomName get_current_location() const
    {
        return _player.get_current_room();
    }

    void put_into_test_mode()
    {
        _test_mode = true;
    }

    [[nodiscard]] Player player() const
    {
        return _player;
    }

private:
    void print_message() const;

    void init();

    void update_message(const std::string& message);

    std::unique_ptr<Map> _map{};

    bool _test_mode{false};

    // current state
    Player _player{};

    std::string _current_message{};
};
} // namespace adv_sk
