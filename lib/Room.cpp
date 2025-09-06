#include "Room.hpp"

namespace adv_sk {

  enum class Direction : std::uint8_t;

  std::optional<RoomName> RoomConnections::get_connection(
      Direction direction) const {
    const auto connection = connections.find(direction);
    if (connection == connections.end()) {
      return std::nullopt;
    }
    return connection->second;
  }

}  // namespace adv_sk
