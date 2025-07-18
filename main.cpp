/**
 * @file main.cpp
 * @brief A simple C++ program that prints "Hello, World!" to the console.
 *
 * This program demonstrates basic input/output operations in C++.
 */

#include "lib_a/HelloWorldClass.hpp"

#include <iostream>
#include <string>

/**
 * @brief The main function of the program.
 *
 * Entry point of the program. Calls the printGreeting() function.
 *
 * @return int Returns 0 on successful execution.
 */
int main() {
  std::cout << lib_a::greeting();
  return 0;
}
