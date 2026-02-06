// Direction unit tests

#include "Direction.hpp"

#include "gtest/gtest.h"

#include <stdexcept>

namespace adv_sk::test {

  // opposite_direction tests
  TEST(Direction, oppositeOfNorthIsSouth) {
    EXPECT_EQ(opposite_direction(Direction::North), Direction::South);
  }

  TEST(Direction, oppositeOfSouthIsNorth) {
    EXPECT_EQ(opposite_direction(Direction::South), Direction::North);
  }

  TEST(Direction, oppositeOfEastIsWest) {
    EXPECT_EQ(opposite_direction(Direction::East), Direction::West);
  }

  TEST(Direction, oppositeOfWestIsEast) {
    EXPECT_EQ(opposite_direction(Direction::West), Direction::East);
  }

  // direction_to_string tests
  TEST(Direction, northToString) {
    EXPECT_EQ(direction_to_string(Direction::North), "North");
  }

  TEST(Direction, southToString) {
    EXPECT_EQ(direction_to_string(Direction::South), "South");
  }

  TEST(Direction, eastToString) {
    EXPECT_EQ(direction_to_string(Direction::East), "East");
  }

  TEST(Direction, westToString) {
    EXPECT_EQ(direction_to_string(Direction::West), "West");
  }

  // string_to_direction tests
  TEST(Direction, stringToNorth) {
    EXPECT_EQ(string_to_direction("North"), Direction::North);
  }

  TEST(Direction, stringToSouth) {
    EXPECT_EQ(string_to_direction("South"), Direction::South);
  }

  TEST(Direction, stringToEast) {
    EXPECT_EQ(string_to_direction("East"), Direction::East);
  }

  TEST(Direction, stringToWest) {
    EXPECT_EQ(string_to_direction("West"), Direction::West);
  }

  TEST(Direction, stringToDirectionInvalidThrows) {
    EXPECT_THROW(string_to_direction("Invalid"), std::runtime_error);
  }

}  // namespace adv_sk::test
