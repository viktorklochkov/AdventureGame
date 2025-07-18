  // Copyright (c) 2025 Dr. Matthias Hölzl. All rights reserved.

#include "Game.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/catch_all.hpp>

using namespace adv_sk;

using namespace Catch::Matchers;


TEST_CASE("Check that tests are executed")
{
    SECTION("Check that the game is started") {
        Game game{};
        game.put_into_test_mode();
        game.start();
        CHECK(game.get_current_message() == "You are in the Grand Hall. It is a vast, echoing chamber.");
        CHECK(game.get_current_location() == "GrandHall");
    }

    SECTION("Check that I can move North") {
        Game game{};
        game.put_into_test_mode();
        game.start();
        game.move(Direction::North);
        CHECK(game.get_current_message() == "You are in the Armoury. Racks of dusty weapons line the walls.");
        CHECK(game.get_current_location() == "Armoury");

    }

    SECTION("Check that I can move South") {
        Game game{};
        game.put_into_test_mode();
        game.start();
        game.move(Direction::North);
        game.move(Direction::South);
        CHECK(game.get_current_message() == "You are in the Grand Hall. It is a vast, echoing chamber.");
        CHECK(game.get_current_location() == "GrandHall");
    }

    SECTION("Check that list of available directions works")
    {
        Game game{};
        game.put_into_test_mode();
        game.start();
        auto list = game.get_available_directions();
        CHECK(list.size() == 1);
        CHECK(list[0] == Direction::North);

        game.move(Direction::North);
        list = game.get_available_directions();
        CHECK(list.size() == 1);
        CHECK(list[0] == Direction::South);
    }

    SECTION("Check that I cannot move in wrong direction")
    {
        Game game{};
        game.put_into_test_mode();
        game.start();

        game.move(Direction::South);
        CHECK_THAT(game.get_current_message(), StartsWith("Wrong direction"));
    }

    SECTION("Check that I can find hidden items")
    {
        Game game{};
        game.put_into_test_mode();
        game.start();

        game.move(Direction::North);
        game.investigate();
        CHECK(game.get_current_message() == "You search the room. You found a rusty sword!\n");
    }

    SECTION("Check that I can collect discovered item")
    {
        Game game{};
        game.put_into_test_mode();
        game.start();

        game.move(Direction::North);
        game.investigate();

        game.take_item("rusty sword");
        game.investigate();
        CHECK(game.get_current_message() == "You search the room. Nothing found!\n");
    }

    SECTION("Check that a room remembers state")
    {
        Game game{};
        game.put_into_test_mode();
        game.start();

        game.move(Direction::North);
        game.investigate();

        game.take_item("rusty sword");

        game.move(Direction::South);
        game.move(Direction::North);
        game.investigate();

        CHECK(game.get_current_message() == "You search the room. Nothing found!\n");
    }

    SECTION("Check that I can see my inventory") {
        Game game{};
        game.put_into_test_mode();
        game.start();

        game.move(Direction::North);
        game.investigate();
        game.take_item("rusty sword");
        game.display_player_inventory();
        CHECK(game.get_current_message() == "Your inventory contains: rusty sword.\n");
    }

    SECTION("Check that I use my items") {
        Game game{};
        game.put_into_test_mode();
        game.start();

        game.investigate();
        game.take_item("golden chalice");

        game.use_item("golden chalice");
        CHECK(game.get_current_message() == "You hold the golden chalice aloft. It glints in the light and feels cool to the touch.\n");
    }

    SECTION("Check that I can drop items") {
        Game game{};
        game.put_into_test_mode();
        game.start();

        game.investigate();
        game.take_item("golden chalice");

        game.drop_item("golden chalice");
        CHECK(game.get_current_message() == "You drop the golden chalice. It fades away in the darkness.\n");
    }




}
