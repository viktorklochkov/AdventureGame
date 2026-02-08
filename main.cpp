/**
 * @file main.cpp
 * @brief
 *
 */

#include "lib/ConsoleInputHandler.h"
#include "lib/Game.hpp"
#include "lib/IInputHandler.hpp"  // for IInputHandler
#include "lib/IMap.hpp"           // for IMap
#include "lib/IPlayer.hpp"        // for IPlayer
#include "lib/Map.hpp"            // for create_map
#include "lib/Player.hpp"         // for Player

#include <memory>   // for unique_ptr, make_unique
#include <utility>  // for move

/**
 * @brief The main function of the program.
 *
 * @return int Returns 0 on successful execution.
 */
int main() {
  auto map = adv_sk::create_map();
  auto player = std::make_unique<adv_sk::Player>();
  std::unique_ptr<adv_sk::IInputHandler> input =
      std::make_unique<adv_sk::ConsoleInputHandler>();

  adv_sk::Game game{std::move(map), std::move(player), std::move(input)};
  game.start();

  return 0;
}
