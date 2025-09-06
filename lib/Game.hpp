//
// Created by Viktor on 14.07.25.
//

#pragma once

#include "Direction.hpp"  // for Direction
#include "Map.hpp"        // for Map
#include "Player.hpp"     // for Player
#include "Room.hpp"       // for RoomName

#include <cstdint>  // for uint8_t
#include <memory>   // for unique_ptr
#include <string>   // for basic_string, string
#include <utility>  // for move
#include <vector>   // for vector

namespace adv_sk {

  enum class Action : std::uint8_t {
    Move,
    Investigate,
    TakeItem,
    DisplayInventory,
    UseItem,
    DropItem,
    Quit,
  };

  class IInputHandler {
   public:
    virtual ~IInputHandler() = default;

    virtual Action get_action() = 0;
    virtual void provide_directions(
        const std::vector<Direction>& directions) = 0;
    virtual Direction get_direction() = 0;
    virtual std::string get_item_name() = 0;

    virtual void provide_message(const std::string& message) = 0;
  };

  class Game {
   public:
    Game() = default;
    explicit Game(std::unique_ptr<Map> map,
                  std::unique_ptr<IInputHandler> input)
        : _map(std::move(map)), _input_handler(std::move(input)) {
    }

    void handle_user_action();

    void start();

    void move(Direction direction);

    void investigate();

    void take_item(const std::string& item_name) const;

    void display_player_inventory();

    void use_item(const std::string& item_name);

    void drop_item(const std::string& item_name);

    [[nodiscard]] std::vector<Direction> get_available_directions() const;

    [[nodiscard]] std::string get_current_message() const {
      return _current_message;
    }

    [[nodiscard]] RoomName get_current_location() const {
      return _player->get_current_room();
    }

    [[nodiscard]] const Player& player() const {
      return *_player;
    }

   private:
    void print_message() const;

    void init();

    void update_message(const std::string& message);

    std::unique_ptr<Map> _map{nullptr};
    std::unique_ptr<Player> _player{std::make_unique<Player>()};
    std::unique_ptr<IInputHandler> _input_handler{nullptr};

    std::string _current_message{};
  };
}  // namespace adv_sk
