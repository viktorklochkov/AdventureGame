#pragma once

#include "IMap.hpp"       // for IMap
#include "Room.hpp"       // for Room
#include "Types.hpp"      // for RoomName
#include "gmock/gmock.h"  // for MOCK_METHOD

#include <optional>  // for optional
#include <string>    // for string

namespace adv_sk::test {

  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  class MockMap : public IMap {
   public:
    MOCK_METHOD(std::optional<RoomName>, next_room,
                (const RoomName& current_room, Direction direction),
                (override));
    MOCK_METHOD(std::string, get_welcome_message, (const RoomName& room),
                (const, override));
    MOCK_METHOD(Room&, get_room, (const RoomName& room), (override));
  };
  // NOLINTEND(misc-non-private-member-variables-in-classes)

}  // namespace adv_sk::test
