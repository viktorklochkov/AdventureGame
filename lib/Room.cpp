#include "Room.hpp"

namespace adv_sk
{
std::optional<RoomName> RoomConnections::get_connection(Direction direction) const
{
    const auto it = connections.find(direction);
    if (it == connections.end())
    {
        return std::nullopt;
    }
    return it->second;
}

} // namespace adv_sk
