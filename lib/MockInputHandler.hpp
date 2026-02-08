#pragma once

#include "IInputHandler.hpp"  // for IInputHandler, Action
#include "gmock/gmock.h"      // for MOCK_METHOD

#include <string>  // for string
#include <vector>  // for vector

namespace adv_sk::test {

  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  class MockInputHandler : public IInputHandler {
   public:
    MOCK_METHOD(Action, get_action, (), (override));
    MOCK_METHOD(void, provide_directions,
                (const std::vector<Direction>& directions), (override));
    MOCK_METHOD(Direction, get_direction, (), (override));
    MOCK_METHOD(std::string, get_item_name, (), (override));
    MOCK_METHOD(void, provide_message, (const std::string& message),
                (override));
  };
  // NOLINTEND(misc-non-private-member-variables-in-classes)

}  // namespace adv_sk::test
