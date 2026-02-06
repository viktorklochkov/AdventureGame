// Copyright (c) 2025 Dr. Matthias Hölzl. All rights reserved.

#include "Game.hpp"

#include "Direction.hpp"  // for Direction
#include "Map.hpp"        // for Map
#include "gtest/gtest.h"  // for Message, TestPartResult, AssertionResult

#include <memory>   // for unique_ptr, make_unique
#include <string>   // for basic_string, string
#include <utility>  // for move
#include <vector>   // for vector

namespace adv_sk::test {

  class TestInputHandler final : public IInputHandler {
   public:
    Action get_action() override {
      return _action;
    }
    void provide_directions(const std::vector<Direction>& directions) override {
    }
    Direction get_direction() override {
      return _direction;
    }
    void provide_message(const std::string& message) override {
      _message = message;
    }
    std::string get_item_name() override {
      return _item_name;
    }
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    Action _action{Action::Quit};
    Direction _direction{};
    std::string _item_name;
    std::string _message;
    // NOLINTEND(misc-non-private-member-variables-in-classes)
  };

  TEST(Game, start) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game const game{nullptr, std::move(input_handler)};

    EXPECT_EQ(input_handler_ptr->_message,
              "You are in the Grand Hall. It is a vast, echoing chamber.");
    EXPECT_EQ(game.get_current_location(), "GrandHall");
  }

  TEST(Game, moveNorth) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    input_handler_ptr->_direction = Direction::North;
    game.move(Direction::North);

    EXPECT_EQ(input_handler_ptr->_message,
              "You are in the Armoury. Racks of dusty weapons line the walls.");
    EXPECT_EQ(game.get_current_location(), "Armoury");
  }

  TEST(Game, moveSouth) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};
    game.move(Direction::North);
    game.move(Direction::South);
    EXPECT_EQ(input_handler_ptr->_message,
              "You are in the Grand Hall. It is a vast, echoing chamber.");
    EXPECT_EQ(game.get_current_location(), "GrandHall");
  }

  TEST(Game, availableDirections) {
    auto input_handler = std::make_unique<TestInputHandler>();
    Game game{nullptr, std::move(input_handler)};
    auto list = game.get_available_directions();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list[0], Direction::North);

    game.move(Direction::North);
    list = game.get_available_directions();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list[0], Direction::South);
  }

  TEST(Game, moveInWrongDirection) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.move(Direction::South);
    EXPECT_TRUE(input_handler_ptr->_message.find("Wrong direction") !=
                std::string::npos);
  }

  TEST(Game, findItems) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.move(Direction::North);
    game.investigate();
    EXPECT_EQ(input_handler_ptr->_message,
              "You search the room. You found a rusty sword!\n");
  }

  TEST(Game, collectItems) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.move(Direction::North);
    game.investigate();

    game.take_item("rusty sword");
    game.investigate();
    EXPECT_EQ(input_handler_ptr->_message,
              "You search the room. Nothing found!\n");
  }

  TEST(Game, hasState) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.move(Direction::North);
    game.investigate();

    game.take_item("rusty sword");

    game.move(Direction::South);
    game.move(Direction::North);
    game.investigate();

    EXPECT_EQ(input_handler_ptr->_message,
              "You search the room. Nothing found!\n");
  }

  TEST(Game, displayInventory) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.move(Direction::North);
    game.investigate();
    game.take_item("rusty sword");
    game.display_player_inventory();
    EXPECT_EQ(input_handler_ptr->_message,
              "Your inventory contains: rusty sword.\n");
  }

  TEST(Game, useItems) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.investigate();
    game.take_item("golden chalice");

    game.use_item("golden chalice");
    EXPECT_EQ(input_handler_ptr->_message,
              "You hold the golden chalice aloft. It glints in the light and "
              "feels cool to the touch.\n");
  }

  TEST(Game, dropItems) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.investigate();
    game.take_item("golden chalice");

    game.drop_item("golden chalice");
    EXPECT_EQ(input_handler_ptr->_message,
              "You drop the golden chalice. It fades away in the darkness.\n");
  }
}  // namespace adv_sk::test
