//
// Created by Viktor on 06.09.25.
//

#pragma once

#include "Game.hpp"

#include <string>    // for string

namespace adv_sk {
class ConsoleInputHandler : public IInputHandler {
  public:
  Action get_action() override;
  void provide_directions(const std::vector<Direction>& directions) override;
  Direction get_direction() override;
  void provide_message(const std::string& message) override;
  std::string get_item_name() override;
private:

};

}
