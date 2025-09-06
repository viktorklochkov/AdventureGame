#pragma once

#include <cstdint>  // for uint8_t
#include <string>

namespace adv_sk {

  enum class Direction : std::uint8_t {
    North,
    South,
    East,
    West,
  };

  Direction opposite_direction(Direction direction);

  std::string direction_to_string(Direction direction);

  Direction string_to_direction(const std::string& direction);

}  // namespace adv_sk
