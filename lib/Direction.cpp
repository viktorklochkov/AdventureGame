#include "Direction.hpp"

#include <stdexcept>

namespace adv_sk {
  Direction opposite_direction(Direction direction) {
    switch (direction) {
      case Direction::North:
        return Direction::South;
      case Direction::South:
        return Direction::North;
      case Direction::East:
        return Direction::West;
      case Direction::West:
        return Direction::East;
    }
    return {};
  }

  std::string direction_to_string(Direction direction) {
    switch (direction) {
      case Direction::North:
        return "North";
      case Direction::South:
        return "South";
      case Direction::East:
        return "East";
      case Direction::West:
        return "West";
    }
    throw std::runtime_error("Unknown direction");
    return "";
  };
  Direction string_to_direction(const std::string& direction) {
    if (direction == "North") {
      return Direction::North;
    }
    if (direction == "South") {
      return Direction::South;
    }
    if (direction == "East") {
      return Direction::East;
    }
    if (direction == "West") {
      return Direction::West;
    }
    throw std::runtime_error("Unknown direction");
  }

}  // namespace adv_sk
