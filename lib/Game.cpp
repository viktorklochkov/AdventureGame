//
// Created by Viktor on 14.07.25.
//

#include "Game.hpp"

#include "Inventory.hpp"  // for InventoryItem

#include <array>     // for array
#include <format>    // for format
#include <optional>  // for optional
#include <ranges>    // for find_if

namespace adv_sk {

  bool Game::handle_user_action() {
    switch (auto action = _input_handler->get_action()) {
      case Action::Quit: {
        return false;
      }
      case Action::Move: {
        _input_handler->provide_directions(get_available_directions());
        move(_input_handler->get_direction());
        break;
      }
      case Action::Investigate: {
        _input_handler->provide_message("Investigating " +
                                        get_current_location());
        investigate();
        break;
      }
      case Action::TakeItem: {
        _input_handler->provide_message("What do you want to take?");
        take_item(_input_handler->get_item_name());
        break;
      }
      case Action::UseItem: {
        _input_handler->provide_message("What do you want to use?");
        use_item(_input_handler->get_item_name());
        break;
      }
      case Action::DropItem: {
        _input_handler->provide_message("What do you want to drop?");
        drop_item(_input_handler->get_item_name());
        break;
      }
      case Action::DisplayInventory: {
        display_player_inventory();
        break;
      }
      default: {
        _input_handler->provide_message("Command not recognized.");
      };
    }
    return true;
  }

  void Game::start() {
    if (!_input_handler) {
      return;
    }
    while (handle_user_action()) {
      // Game loop continues until Action::Quit
    }
  }

  void Game::move(Direction direction) {
    if (const auto next_room =
            _map->next_room(_player->get_current_room(), direction);
        next_room.has_value()) {
      _player->change_room(next_room.value());
      update_message(_map->get_welcome_message(_player->get_current_room()));
    } else {
      update_message("Wrong direction!\n");
    }
  }

  void Game::investigate() {
    const auto room = _player->get_current_room();
    if (auto& inventory = _map->get_room(room).inventory();
        !inventory.empty()) {
      std::string message = "You search the room. You found";
      for (auto& item : inventory) {
        item.is_visible = true;
        message.append(" a " + item.name);
      }
      message.append("!\n");
      update_message(message);
    } else {
      update_message("You search the room. Nothing found!\n");
    }
  }

  void Game::take_item(const std::string& item_name) {
    const auto room = _player->get_current_room();
    auto& inventory = _map->get_room(room).inventory();
    const auto item = std::ranges::find_if(
        inventory, [&item_name](const InventoryItem& item) {
          return item.name == item_name && item.is_visible;
        });
    if (item != inventory.end()) {
      update_message(std::format("You take the {}\n", item->name));
      _player->add_to_inventory(*item);
      inventory.erase(item);
    } else {
      update_message(std::format("You can't take the {}\n", item_name));
    }
  }

  void Game::display_player_inventory() {
    std::string message("Your inventory contains:");
    for (const auto& item : _player->get_inventory()) {
      message.append(" " + item.name);
    }
    message.append(".\n");
    update_message(message);
  }

  void Game::use_item(const std::string& item_name) {
    auto& inventory = _player->get_mutable_inventory();
    const auto item = std::ranges::find_if(
        inventory, [&item_name](const InventoryItem& item) {
          return item.name == item_name;
        });
    if (item != inventory.end()) {
      update_message(item->use_message);
      inventory.erase(item);
    } else {
      update_message("You can't use the " + item_name + "!\n");
    }
  }

  void Game::drop_item(const std::string& item_name) {
    auto& inventory = _player->get_mutable_inventory();
    const auto item = std::ranges::find_if(
        inventory, [&item_name](const InventoryItem& item) {
          return item.name == item_name;
        });
    if (item != inventory.end()) {
      update_message(std::format(
          "You drop the {}. It fades away in the darkness.\n", item_name));
      _map->get_room(_player->get_current_room()).add_to_inventory(*item);
      inventory.erase(item);
    } else {
      update_message("You can't drop the " + item_name + "!\n");
    }
  }

  std::vector<Direction> Game::get_available_directions() const {
    std::vector<Direction> result;
    for (auto direction : ALL_DIRECTIONS) {
      auto room = _map->next_room(_player->get_current_room(), direction);
      if (room.has_value()) {
        result.push_back(direction);
      }
    }
    return result;
  }

  void Game::init() {
    // default map
    if (_map == nullptr) {
      _map = create_map();
    }
    _player->change_room("GrandHall");
  }

  void Game::update_message(const std::string& message) {
    if (_input_handler) {
      _input_handler->provide_message(message);
    } else {
      _current_message = message;
    }
  }
}  // namespace adv_sk
