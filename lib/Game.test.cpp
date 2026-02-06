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

  // Error case tests
  TEST(Game, takeItemNotVisible) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    // Try to take item without investigating first (item not visible)
    game.take_item("golden chalice");
    EXPECT_EQ(input_handler_ptr->_message,
              "You can't take the golden chalice\n");
  }

  TEST(Game, takeItemNotExists) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.investigate();
    game.take_item("nonexistent item");
    EXPECT_EQ(input_handler_ptr->_message,
              "You can't take the nonexistent item\n");
  }

  TEST(Game, useItemNotInInventory) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.use_item("nonexistent item");
    EXPECT_EQ(input_handler_ptr->_message,
              "You can't use the nonexistent item!\n");
  }

  TEST(Game, dropItemNotInInventory) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler const* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.drop_item("nonexistent item");
    EXPECT_EQ(input_handler_ptr->_message,
              "You can't drop the nonexistent item!\n");
  }

  // Game loop tests via handle_user_action
  TEST(Game, handleUserActionQuit) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    input_handler_ptr->_action = Action::Quit;
    EXPECT_FALSE(game.handle_user_action());
  }

  TEST(Game, handleUserActionMove) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    input_handler_ptr->_action = Action::Move;
    input_handler_ptr->_direction = Direction::North;
    EXPECT_TRUE(game.handle_user_action());
    EXPECT_EQ(game.get_current_location(), "Armoury");
  }

  TEST(Game, handleUserActionInvestigate) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    input_handler_ptr->_action = Action::Investigate;
    EXPECT_TRUE(game.handle_user_action());
    EXPECT_TRUE(input_handler_ptr->_message.find("You search the room") !=
                std::string::npos);
  }

  TEST(Game, handleUserActionTakeItem) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.investigate();  // Make items visible
    input_handler_ptr->_action = Action::TakeItem;
    input_handler_ptr->_item_name = "golden chalice";
    EXPECT_TRUE(game.handle_user_action());
    EXPECT_EQ(input_handler_ptr->_message, "You take the golden chalice\n");
  }

  TEST(Game, handleUserActionUseItem) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.investigate();
    game.take_item("golden chalice");
    input_handler_ptr->_action = Action::UseItem;
    input_handler_ptr->_item_name = "golden chalice";
    EXPECT_TRUE(game.handle_user_action());
  }

  TEST(Game, handleUserActionDropItem) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    game.investigate();
    game.take_item("golden chalice");
    input_handler_ptr->_action = Action::DropItem;
    input_handler_ptr->_item_name = "golden chalice";
    EXPECT_TRUE(game.handle_user_action());
  }

  TEST(Game, handleUserActionDisplayInventory) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    input_handler_ptr->_action = Action::DisplayInventory;
    EXPECT_TRUE(game.handle_user_action());
    EXPECT_TRUE(input_handler_ptr->_message.find("Your inventory contains") !=
                std::string::npos);
  }

  TEST(Game, gameLoopRunsUntilQuit) {
    auto input_handler = std::make_unique<TestInputHandler>();
    TestInputHandler* input_handler_ptr = input_handler.get();
    Game game{nullptr, std::move(input_handler)};

    // First action: investigate, then quit
    input_handler_ptr->_action = Action::Quit;
    game.start();  // Should return immediately after Quit action
    // If we get here without hanging, the test passes
    EXPECT_EQ(game.get_current_location(), "GrandHall");
  }

}  // namespace adv_sk::test
