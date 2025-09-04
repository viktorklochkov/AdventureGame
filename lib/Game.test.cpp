// Copyright (c) 2025 Dr. Matthias Hölzl. All rights reserved.

#include "Game.hpp"
#include "Direction.hpp"// for Direction

#include "gtest/gtest.h"// for Message, TestPartResult, AssertionResult

#include <string>// for basic_string, string
#include <vector>// for vector

using adv_sk::Direction;
using adv_sk::Game;

TEST(Game, start) {
  Game game{};
  game.put_into_test_mode();
  game.start();
  EXPECT_EQ(game.get_current_message(), "You are in the Grand Hall. It is a vast, echoing chamber.");
  EXPECT_EQ(game.get_current_location(), "GrandHall");
}

TEST(Game, moveNorth) {
  Game game{};
  game.put_into_test_mode();
  game.start();
  game.move(Direction::North);
  EXPECT_EQ(game.get_current_message(), "You are in the Armoury. Racks of dusty weapons line the walls.");
  EXPECT_EQ(game.get_current_location(), "Armoury");
}

TEST(Game, moveSouth) {
  Game game{};
  game.put_into_test_mode();
  game.start();
  game.move(Direction::North);
  game.move(Direction::South);
  EXPECT_EQ(game.get_current_message(), "You are in the Grand Hall. It is a vast, echoing chamber.");
  EXPECT_EQ(game.get_current_location(), "GrandHall");
}

TEST(Game, availableDirections) {
  Game game{};
  game.put_into_test_mode();
  game.start();
  auto list = game.get_available_directions();
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list[0], Direction::North);

  game.move(Direction::North);
  list = game.get_available_directions();
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list[0], Direction::South);
}

TEST(Game, moveInWrongDirection) {
  Game game{};
  game.put_into_test_mode();
  game.start();

  game.move(Direction::South);
  EXPECT_TRUE(game.get_current_message().find("Wrong direction") != std::string::npos);
}

TEST(Game, findItems) {
  Game game{};
  game.put_into_test_mode();
  game.start();

  game.move(Direction::North);
  game.investigate();
  EXPECT_EQ(game.get_current_message(), "You search the room. You found a rusty sword!\n");
}

TEST(Game, collectItems) {
  Game game{};
  game.put_into_test_mode();
  game.start();

  game.move(Direction::North);
  game.investigate();

  game.take_item("rusty sword");
  game.investigate();
  EXPECT_EQ(game.get_current_message(), "You search the room. Nothing found!\n");
}

TEST(Game, hasState) {
  Game game{};
  game.put_into_test_mode();
  game.start();

  game.move(Direction::North);
  game.investigate();

  game.take_item("rusty sword");

  game.move(Direction::South);
  game.move(Direction::North);
  game.investigate();

  EXPECT_EQ(game.get_current_message(), "You search the room. Nothing found!\n");
}

TEST(Game, displayInventory) {
  Game game{};
  game.put_into_test_mode();
  game.start();

  game.move(Direction::North);
  game.investigate();
  game.take_item("rusty sword");
  game.display_player_inventory();
  EXPECT_EQ(game.get_current_message(), "Your inventory contains: rusty sword.\n");
}

TEST(Game, useItems) {
  Game game{};
  game.put_into_test_mode();
  game.start();

  game.investigate();
  game.take_item("golden chalice");

  game.use_item("golden chalice");
  EXPECT_EQ(game.get_current_message(), "You hold the golden chalice aloft. It glints in the light and feels cool to the touch.\n");
}

TEST(Game, dropItems) {
  Game game{};
  game.put_into_test_mode();
  game.start();

  game.investigate();
  game.take_item("golden chalice");

  game.drop_item("golden chalice");
  EXPECT_EQ(game.get_current_message(), "You drop the golden chalice. It fades away in the darkness.\n");
}
