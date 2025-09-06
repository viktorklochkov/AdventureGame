//
// Created by Viktor on 06.09.25.
//

#include "ConsoleInputHandler.h"

#include "Direction.hpp"  // for direction_to_string, string_to_...

#include <iostream>
#include <vector>  // for vector

namespace adv_sk {

  namespace {
    Action string_to_action(const std::string& action) {
      if (action == "quit") {
        return Action::Quit;
      }
      if (action == "move") {
        return Action::Move;
      }
      if (action == "use") {
        return Action::UseItem;
      }
      if (action == "investigate") {
        return Action::Investigate;
      }
      if (action == "take") {
        return Action::TakeItem;
      }

      return Action::Quit;
    }
  }  // namespace

  Action ConsoleInputHandler::get_action() {
    std::string action;
    std::cout << "Enter action (quit to exit): ";
    std::getline(std::cin, action);
    return string_to_action(action);
  }
  void ConsoleInputHandler::provide_directions(
      const std::vector<Direction>& directions) {
    std::cout << "Available directions:\n";
    for (const auto& direction : directions) {
      std::cout << "- " << direction_to_string(direction) << '\n';
    }
  }
  Direction ConsoleInputHandler::get_direction() {
    std::string input;
    std::cout << "Choose direction: ";
    std::getline(std::cin, input);
    return string_to_direction(input);
  }

  void ConsoleInputHandler::provide_message(const std::string& message) {
    std::cout << message << "\n";
  }
  std::string ConsoleInputHandler::get_item_name() {
    std::string input;
    std::getline(std::cin, input);
    return input;
  }

}  // namespace adv_sk