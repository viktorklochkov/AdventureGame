// ConsoleInputHandler unit tests with stream redirection

#include "ConsoleInputHandler.h"

#include "Direction.hpp"      // for Direction
#include "IInputHandler.hpp"  // for Action
#include "gtest/gtest.h"      // for TEST, EXPECT_EQ

#include <iostream>  // for cin, cout
#include <sstream>   // for istringstream, ostringstream
#include <string>    // for string
#include <vector>    // for vector

namespace adv_sk::test {

  namespace {
    class StreamRedirector {
     public:
      explicit StreamRedirector(const std::string& input)
          : _input(input),
            _old_cin(std::cin.rdbuf(_input.rdbuf())),
            _old_cout(std::cout.rdbuf(_output.rdbuf())) {
      }

      ~StreamRedirector() {  // NOLINT(modernize-use-equals-default)
        std::cin.rdbuf(_old_cin);
        std::cout.rdbuf(_old_cout);
      }

      StreamRedirector(const StreamRedirector&) = delete;
      StreamRedirector& operator=(const StreamRedirector&) = delete;
      StreamRedirector(StreamRedirector&&) = delete;
      StreamRedirector& operator=(StreamRedirector&&) = delete;

      [[nodiscard]] std::string output() const {
        return _output.str();
      }

     private:
      std::istringstream _input;
      std::ostringstream _output;
      std::streambuf* _old_cin;
      std::streambuf* _old_cout;
    };
  }  // namespace

  TEST(ConsoleInputHandler, getActionQuit) {
    const StreamRedirector redirect("quit\n");
    ConsoleInputHandler handler;
    EXPECT_EQ(handler.get_action(), Action::Quit);
  }

  TEST(ConsoleInputHandler, getActionMove) {
    const StreamRedirector redirect("move\n");
    ConsoleInputHandler handler;
    EXPECT_EQ(handler.get_action(), Action::Move);
  }

  TEST(ConsoleInputHandler, getActionInvestigate) {
    const StreamRedirector redirect("investigate\n");
    ConsoleInputHandler handler;
    EXPECT_EQ(handler.get_action(), Action::Investigate);
  }

  TEST(ConsoleInputHandler, getActionTake) {
    const StreamRedirector redirect("take\n");
    ConsoleInputHandler handler;
    EXPECT_EQ(handler.get_action(), Action::TakeItem);
  }

  TEST(ConsoleInputHandler, getActionUse) {
    const StreamRedirector redirect("use\n");
    ConsoleInputHandler handler;
    EXPECT_EQ(handler.get_action(), Action::UseItem);
  }

  TEST(ConsoleInputHandler, getActionUnknownDefaultsToQuit) {
    const StreamRedirector redirect("unknown\n");
    ConsoleInputHandler handler;
    EXPECT_EQ(handler.get_action(), Action::Quit);
  }

  TEST(ConsoleInputHandler, provideDirections) {
    const StreamRedirector redirect("");
    ConsoleInputHandler handler;
    handler.provide_directions({Direction::North, Direction::South});
    const std::string result = redirect.output();
    EXPECT_TRUE(result.find("North") != std::string::npos);
    EXPECT_TRUE(result.find("South") != std::string::npos);
  }

  TEST(ConsoleInputHandler, getDirection) {
    const StreamRedirector redirect("North\n");
    ConsoleInputHandler handler;
    EXPECT_EQ(handler.get_direction(), Direction::North);
  }

  TEST(ConsoleInputHandler, provideMessage) {
    const StreamRedirector redirect("");
    ConsoleInputHandler handler;
    handler.provide_message("Hello World");
    EXPECT_TRUE(redirect.output().find("Hello World") != std::string::npos);
  }

  TEST(ConsoleInputHandler, getItemName) {
    const StreamRedirector redirect("golden chalice\n");
    ConsoleInputHandler handler;
    EXPECT_EQ(handler.get_item_name(), "golden chalice");
  }

}  // namespace adv_sk::test
