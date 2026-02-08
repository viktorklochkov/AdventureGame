// Game London-style unit tests

#include "Game.hpp"

#include "Direction.hpp"         // for Direction
#include "IInputHandler.hpp"     // for Action, IInputHandler
#include "IMap.hpp"              // for IMap
#include "IPlayer.hpp"           // for IPlayer
#include "Inventory.hpp"         // for InventoryItem
#include "MockInputHandler.hpp"  // for MockInputHandler
#include "MockMap.hpp"           // for MockMap
#include "MockPlayer.hpp"        // for MockPlayer
#include "Room.hpp"              // for Room
#include "Types.hpp"             // for RoomName
#include "gmock/gmock.h"         // for NiceMock, Return, ReturnRef
#include "gtest/gtest.h"         // for TEST_F, EXPECT_CALL

#include <memory>    // for unique_ptr, make_unique
#include <optional>  // for optional, nullopt
#include <string>    // for string
#include <utility>   // for move
#include <vector>    // for vector

namespace adv_sk::test {

  using ::testing::_;
  using ::testing::NiceMock;
  using ::testing::Return;
  using ::testing::ReturnRef;

  // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
  class GameTest : public ::testing::Test {
   protected:
    void SetUp() override {
      auto map = std::make_unique<NiceMock<MockMap>>();
      auto player = std::make_unique<NiceMock<MockPlayer>>();
      auto input = std::make_unique<NiceMock<MockInputHandler>>();

      mock_map = map.get();
      mock_player = player.get();
      mock_input = input.get();

      ON_CALL(*mock_player, get_current_room())
          .WillByDefault(Return("GrandHall"));
      ON_CALL(*mock_map, get_welcome_message(_))
          .WillByDefault(Return("Welcome to GrandHall"));

      game = std::make_unique<Game>(std::move(map), std::move(player),
                                    std::move(input));
    }

    NiceMock<MockMap>* mock_map = nullptr;
    NiceMock<MockPlayer>* mock_player = nullptr;
    NiceMock<MockInputHandler>* mock_input = nullptr;
    std::unique_ptr<Game> game;
  };
  // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)

  TEST(GameConstruction, setsStartingRoomAndSendsWelcomeMessage) {
    auto map = std::make_unique<NiceMock<MockMap>>();
    auto player = std::make_unique<NiceMock<MockPlayer>>();
    auto input = std::make_unique<NiceMock<MockInputHandler>>();
    auto* player_ptr = player.get();
    auto* map_ptr = map.get();
    auto* input_ptr = input.get();

    ON_CALL(*player_ptr, get_current_room()).WillByDefault(Return("GrandHall"));
    ON_CALL(*map_ptr, get_welcome_message(_)).WillByDefault(Return("Welcome"));

    EXPECT_CALL(*player_ptr, change_room("GrandHall")).Times(1);
    EXPECT_CALL(*map_ptr, get_welcome_message("GrandHall")).Times(1);
    EXPECT_CALL(*input_ptr, provide_message("Welcome")).Times(1);

    const Game game_obj(std::move(map), std::move(player), std::move(input));
  }

  // --- move() tests ---

  TEST_F(GameTest, moveToValidRoomChangesPlayerRoom) {
    EXPECT_CALL(*mock_player, get_current_room())
        .WillOnce(Return("GrandHall"))
        .WillOnce(Return("Armoury"));
    EXPECT_CALL(*mock_map, next_room("GrandHall", Direction::North))
        .WillOnce(Return(std::optional<RoomName>("Armoury")));
    EXPECT_CALL(*mock_player, change_room("Armoury"));
    EXPECT_CALL(*mock_map, get_welcome_message("Armoury"))
        .WillOnce(Return("Welcome to Armoury"));
    EXPECT_CALL(*mock_input, provide_message("Welcome to Armoury"));

    game->move(Direction::North);
  }

  TEST_F(GameTest, moveToInvalidDirectionShowsError) {
    EXPECT_CALL(*mock_player, get_current_room()).WillOnce(Return("GrandHall"));
    EXPECT_CALL(*mock_map, next_room("GrandHall", Direction::South))
        .WillOnce(Return(std::nullopt));
    EXPECT_CALL(*mock_input, provide_message("Wrong direction!\n"));

    game->move(Direction::South);
  }

  // --- investigate() tests ---

  TEST_F(GameTest, investigateRevealsItems) {
    Room room("TestRoom", "msg", {InventoryItem{.name = "sword"}});
    EXPECT_CALL(*mock_player, get_current_room()).WillOnce(Return("TestRoom"));
    EXPECT_CALL(*mock_map, get_room("TestRoom")).WillOnce(ReturnRef(room));
    EXPECT_CALL(*mock_input,
                provide_message("You search the room. You found a sword!\n"));

    game->investigate();
    EXPECT_TRUE(room.inventory()[0].is_visible);
  }

  TEST_F(GameTest, investigateEmptyRoomShowsNothing) {
    Room room("TestRoom", "msg");
    EXPECT_CALL(*mock_player, get_current_room()).WillOnce(Return("TestRoom"));
    EXPECT_CALL(*mock_map, get_room("TestRoom")).WillOnce(ReturnRef(room));
    EXPECT_CALL(*mock_input,
                provide_message("You search the room. Nothing found!\n"));

    game->investigate();
  }

  // --- take_item() tests ---

  TEST_F(GameTest, takeVisibleItemAddsToPlayerInventory) {
    const InventoryItem sword{
        .name = "sword", .use_message = "", .is_visible = true};
    Room room("R", "", {sword});
    EXPECT_CALL(*mock_player, get_current_room()).WillOnce(Return("R"));
    EXPECT_CALL(*mock_map, get_room("R")).WillOnce(ReturnRef(room));
    EXPECT_CALL(*mock_input, provide_message("You take the sword\n"));
    EXPECT_CALL(*mock_player, add_to_inventory(_));

    game->take_item("sword");
    EXPECT_TRUE(room.inventory().empty());
  }

  TEST_F(GameTest, takeInvisibleItemFails) {
    const InventoryItem sword{
        .name = "sword", .use_message = "", .is_visible = false};
    Room room("R", "", {sword});
    EXPECT_CALL(*mock_player, get_current_room()).WillOnce(Return("R"));
    EXPECT_CALL(*mock_map, get_room("R")).WillOnce(ReturnRef(room));
    EXPECT_CALL(*mock_input, provide_message("You can't take the sword\n"));

    game->take_item("sword");
  }

  TEST_F(GameTest, takeNonexistentItemFails) {
    Room room("R", "");
    EXPECT_CALL(*mock_player, get_current_room()).WillOnce(Return("R"));
    EXPECT_CALL(*mock_map, get_room("R")).WillOnce(ReturnRef(room));
    EXPECT_CALL(*mock_input, provide_message("You can't take the ghost\n"));

    game->take_item("ghost");
  }

  // --- use_item() tests ---

  TEST_F(GameTest, useItemInInventoryShowsMessage) {
    std::vector<InventoryItem> inv{{.name = "potion",
                                    .use_message = "You drink it!\n",
                                    .is_visible = true}};
    EXPECT_CALL(*mock_player, get_mutable_inventory()).WillOnce(ReturnRef(inv));
    EXPECT_CALL(*mock_input, provide_message("You drink it!\n"));

    game->use_item("potion");
    EXPECT_TRUE(inv.empty());
  }

  TEST_F(GameTest, useItemNotInInventoryFails) {
    std::vector<InventoryItem> inv;
    EXPECT_CALL(*mock_player, get_mutable_inventory()).WillOnce(ReturnRef(inv));
    EXPECT_CALL(*mock_input, provide_message("You can't use the ghost!\n"));

    game->use_item("ghost");
  }

  // --- drop_item() tests ---

  TEST_F(GameTest, dropItemMovesToRoom) {
    const InventoryItem sword{
        .name = "sword", .use_message = "", .is_visible = true};
    std::vector<InventoryItem> inv{sword};
    Room room("R");
    EXPECT_CALL(*mock_player, get_mutable_inventory()).WillOnce(ReturnRef(inv));
    EXPECT_CALL(*mock_player, get_current_room()).WillOnce(Return("R"));
    EXPECT_CALL(*mock_map, get_room("R")).WillOnce(ReturnRef(room));
    EXPECT_CALL(*mock_input,
                provide_message(
                    "You drop the sword. It fades away in the darkness.\n"));

    game->drop_item("sword");
    EXPECT_TRUE(inv.empty());
    EXPECT_EQ(room.inventory().size(), 1);
  }

  TEST_F(GameTest, dropItemNotInInventoryFails) {
    std::vector<InventoryItem> inv;
    EXPECT_CALL(*mock_player, get_mutable_inventory()).WillOnce(ReturnRef(inv));
    EXPECT_CALL(*mock_input, provide_message("You can't drop the ghost!\n"));

    game->drop_item("ghost");
  }

  // --- display_player_inventory() tests ---

  TEST_F(GameTest, displayInventoryShowsItems) {
    std::vector<InventoryItem> inv{{.name = "sword"}, {.name = "shield"}};
    EXPECT_CALL(*mock_player, get_inventory()).WillOnce(ReturnRef(inv));
    EXPECT_CALL(*mock_input,
                provide_message("Your inventory contains: sword shield.\n"));

    game->display_player_inventory();
  }

  TEST_F(GameTest, displayEmptyInventory) {
    std::vector<InventoryItem> inv;
    EXPECT_CALL(*mock_player, get_inventory()).WillOnce(ReturnRef(inv));
    EXPECT_CALL(*mock_input, provide_message("Your inventory contains:.\n"));

    game->display_player_inventory();
  }

  // --- get_available_directions() tests ---

  TEST_F(GameTest, getAvailableDirectionsReturnsValidOnes) {
    EXPECT_CALL(*mock_player, get_current_room())
        .WillRepeatedly(Return("GrandHall"));
    EXPECT_CALL(*mock_map, next_room("GrandHall", Direction::North))
        .WillOnce(Return(std::optional<RoomName>("Armoury")));
    EXPECT_CALL(*mock_map, next_room("GrandHall", Direction::South))
        .WillOnce(Return(std::nullopt));
    EXPECT_CALL(*mock_map, next_room("GrandHall", Direction::East))
        .WillOnce(Return(std::nullopt));
    EXPECT_CALL(*mock_map, next_room("GrandHall", Direction::West))
        .WillOnce(Return(std::nullopt));

    auto dirs = game->get_available_directions();
    ASSERT_EQ(dirs.size(), 1);
    EXPECT_EQ(dirs[0], Direction::North);
  }

  TEST_F(GameTest, getAvailableDirectionsReturnsEmpty) {
    EXPECT_CALL(*mock_player, get_current_room())
        .WillRepeatedly(Return("Isolated"));
    EXPECT_CALL(*mock_map, next_room("Isolated", _))
        .WillRepeatedly(Return(std::nullopt));

    auto dirs = game->get_available_directions();
    EXPECT_TRUE(dirs.empty());
  }

  // --- get_current_location() ---

  TEST_F(GameTest, getCurrentLocationDelegatesToPlayer) {
    EXPECT_CALL(*mock_player, get_current_room()).WillOnce(Return("TestRoom"));

    EXPECT_EQ(game->get_current_location(), "TestRoom");
  }

  // --- handle_user_action() tests ---

  TEST_F(GameTest, handleUserActionQuitReturnsFalse) {
    EXPECT_CALL(*mock_input, get_action()).WillOnce(Return(Action::Quit));

    EXPECT_FALSE(game->handle_user_action());
  }

  TEST_F(GameTest, handleUserActionMoveCallsSequence) {
    EXPECT_CALL(*mock_input, get_action()).WillOnce(Return(Action::Move));
    EXPECT_CALL(*mock_input, provide_directions(_));
    EXPECT_CALL(*mock_input, get_direction())
        .WillOnce(Return(Direction::North));
    EXPECT_CALL(*mock_player, get_current_room())
        .WillRepeatedly(Return("GrandHall"));
    EXPECT_CALL(*mock_map, next_room(_, _))
        .WillRepeatedly(Return(std::nullopt));

    EXPECT_TRUE(game->handle_user_action());
  }

  TEST_F(GameTest, handleUserActionInvestigate) {
    Room room("R");
    EXPECT_CALL(*mock_input, get_action())
        .WillOnce(Return(Action::Investigate));
    EXPECT_CALL(*mock_player, get_current_room()).WillRepeatedly(Return("R"));
    EXPECT_CALL(*mock_map, get_room("R")).WillOnce(ReturnRef(room));

    EXPECT_TRUE(game->handle_user_action());
  }

  TEST_F(GameTest, handleUserActionTakeItem) {
    Room room("R");
    EXPECT_CALL(*mock_input, get_action()).WillOnce(Return(Action::TakeItem));
    EXPECT_CALL(*mock_input, get_item_name()).WillOnce(Return("sword"));
    EXPECT_CALL(*mock_player, get_current_room()).WillRepeatedly(Return("R"));
    EXPECT_CALL(*mock_map, get_room("R")).WillOnce(ReturnRef(room));

    EXPECT_TRUE(game->handle_user_action());
  }

  TEST_F(GameTest, handleUserActionUseItem) {
    std::vector<InventoryItem> inv;
    EXPECT_CALL(*mock_input, get_action()).WillOnce(Return(Action::UseItem));
    EXPECT_CALL(*mock_input, get_item_name()).WillOnce(Return("potion"));
    EXPECT_CALL(*mock_player, get_mutable_inventory()).WillOnce(ReturnRef(inv));

    EXPECT_TRUE(game->handle_user_action());
  }

  TEST_F(GameTest, handleUserActionDropItem) {
    std::vector<InventoryItem> inv;
    EXPECT_CALL(*mock_input, get_action()).WillOnce(Return(Action::DropItem));
    EXPECT_CALL(*mock_input, get_item_name()).WillOnce(Return("sword"));
    EXPECT_CALL(*mock_player, get_mutable_inventory()).WillOnce(ReturnRef(inv));

    EXPECT_TRUE(game->handle_user_action());
  }

  TEST_F(GameTest, handleUserActionDisplayInventory) {
    std::vector<InventoryItem> inv;
    EXPECT_CALL(*mock_input, get_action())
        .WillOnce(Return(Action::DisplayInventory));
    EXPECT_CALL(*mock_player, get_inventory()).WillOnce(ReturnRef(inv));

    EXPECT_TRUE(game->handle_user_action());
  }

  // --- start() tests ---

  TEST_F(GameTest, startReturnsImmediatelyOnQuit) {
    EXPECT_CALL(*mock_input, get_action()).WillOnce(Return(Action::Quit));

    game->start();
  }

  TEST(GameDefaultConstruction, startWithNullInputReturns) {
    Game empty_game;
    empty_game.start();
  }

}  // namespace adv_sk::test
