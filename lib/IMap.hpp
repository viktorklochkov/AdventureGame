#pragma once

#include "Types.hpp"  // for RoomName

#include <cstdint>   // for uint8_t
#include <optional>  // for optional
#include <string>    // for string

namespace adv_sk {

  enum class Direction : std::uint8_t;
  class Room;

  class IMap {
   public:
    virtual ~IMap() = default;

    virtual std::optional<RoomName> next_room(const RoomName& current_room,
                                              Direction direction) = 0;

    [[nodiscard]] virtual std::string get_welcome_message(
        const RoomName& room) const = 0;

    [[nodiscard]] virtual Room& get_room(const RoomName& room) = 0;
  };

}  // namespace adv_sk
