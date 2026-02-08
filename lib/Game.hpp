//
// Created by Viktor on 14.07.25.
//

#pragma once

#include "Direction.hpp"      // for Direction
#include "IInputHandler.hpp"  // for IInputHandler, Action
#include "IMap.hpp"           // for IMap
#include "IPlayer.hpp"        // for IPlayer
#include "Types.hpp"          // for RoomName

#include <memory>   // for unique_ptr
#include <string>   // for string
#include <utility>  // for move
#include <vector>   // for vector

namespace adv_sk {

  class Game {
   public:
    Game() = default;
    explicit Game(std::unique_ptr<IMap> map, std::unique_ptr<IPlayer> player,
                  std::unique_ptr<IInputHandler> input)
        : _map(std::move(map)),
          _player(std::move(player)),
          _input_handler(std::move(input)) {
      _player->change_room("GrandHall");
      update_message(_map->get_welcome_message(_player->get_current_room()));
    }

    [[nodiscard]] bool handle_user_action();

    void start();

    void move(Direction direction);

    void investigate();

    void take_item(const std::string& item_name);

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

   private:
    void update_message(const std::string& message);

    std::unique_ptr<IMap> _map{nullptr};
    std::unique_ptr<IPlayer> _player{nullptr};
    std::unique_ptr<IInputHandler> _input_handler{nullptr};

    std::string _current_message{};
  };

}  // namespace adv_sk
