#pragma once

#include "Direction.hpp"  // for Direction

#include <cstdint>  // for uint8_t
#include <string>   // for string
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

}  // namespace adv_sk
