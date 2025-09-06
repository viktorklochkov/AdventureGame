/**
 * @file main.cpp
 * @brief
 *
 */

#include "lib/ConsoleInputHandler.h"
#include "lib/Game.hpp"
#include "lib/Map.hpp"  // for Map, create_map

#include <memory>   // for unique_ptr, make_unique
#include <utility>  // for move

/**
 * @brief The main function of the program.
 *
 * @return int Returns 0 on successful execution.
 */
int main() {
  auto map = adv_sk::create_map();
  std::unique_ptr<adv_sk::IInputHandler> input =
      std::make_unique<adv_sk::ConsoleInputHandler>();

  adv_sk::Game game{std::move(map), std::move(input)};
  game.start();

  return 0;
}
