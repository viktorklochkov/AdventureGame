#pragma once

#include <array>
#include <cstdint>  // for uint8_t
#include <string>

namespace adv_sk {

  enum class Direction : std::uint8_t {
    North,
    South,
    East,
    West,
  };

  inline constexpr std::array<Direction, 4> ALL_DIRECTIONS{
      Direction::North, Direction::South, Direction::East, Direction::West};

  Direction opposite_direction(Direction direction);

  std::string direction_to_string(Direction direction);

  Direction string_to_direction(const std::string& direction);

}  // namespace adv_sk
